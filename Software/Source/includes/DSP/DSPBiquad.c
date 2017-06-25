#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <sys/types.h>

//error
#include "../error/error.h"

#include <math.h>
#include "DSPDefine.h"
#include "DSPMath.h"
#include "DSPI2C.h"

//I2C
#include <linux/i2c-dev.h>
#include "../libi2c/libi2c.h"

//INI
#include "../libini/libini.h"

//DSP
#include "../Sigma/v122param.h"

//Config
#include "../../config.h"

#define PI 						3.141592654
#define Fs 						48000.0

/* Crossover biquad calc section
Source : https://ez.analog.com/servlet/JiveServlet/download/1819-2-6143/FilterMathCalculations.pdf
*/

/* ADAU1701_ReadBiquad
* Input : DSP Address |First register address
* Output: Return a biquad of 5 coeficients (5 addresses)
*/

void ADAU1701_ReadBiquad	(int DSPAddress,int register_address,struct Biquad* biquad)
{
	// Biquad

	char* Filt_Descr[5] = {"b0","b1","b2","a1","a2"};
	int i;
	
/// To DO mecanisme devérification 
		
	for(i=0;i<5;i++)
	{
		//Initialisation des adresses
		biquad->addr[i] = register_address + i;

		//Initialisation des descripteurs
		biquad->description[i] = Filt_Descr[i];

		//Récupération des valeurs des biquads
		unsigned long int iValue =  ADAU1701_Read_register(DSPAddress,biquad->addr[i]);
		biquad->value[i] = FixpointToFloat( iValue );
		
		// Complément à 2
		if(iValue > pow(2,27))
		{
			iValue = (~iValue + 1) & 0xFFFFFFF ;
			biquad->value[i] =  FixpointToFloat( iValue ) * (-1);
		
		}
	/*	
		if(i==3 || i==4)
		{
			// inverser le signe de a1 et a2
			biquad->value[i] =  biquad->value[i] * (-1);
		}
	*/	
	}
	
}

f_return checkchannelconfig		(struct channel Ch)
{
	struct f_return err;
	
	if (Ch.mute != 0 && Ch.mute != 1) 
	{
		err = seterror(3,-1,"Mute value not valid");

	}
	if (Ch.phase != -1 && Ch.phase != 1) 
	{
		err = seterror(3,-1,"Phase value not valid");

	}
	if (Ch.delay < 1 && Ch.phase > 412) 
	{
		err = seterror(3,-1,"delay value not valid");

	}
	if (Ch.volume > 0 || Ch.volume < -80) 
	{
		err = seterror(3,-1,"volume out of range");
	}

	
	return err;
}
f_return checkfilterrange		(struct Filter EQ)
{
	f_return err;
	
	err = seterror(0,0,"OK");
	
	#define isNot(name) strcmp(EQ.type,name) !=0
	
	if(strcmp(EQ.type,"HSF")==0 || strcmp(EQ.type,"LSF")==0)
	{
		
		if (EQ.S <= 0 || EQ.S > 2) 
		{
			err = seterror(2,-1,"S out of range");
		}
	}
	else
	{
		if (EQ.Q <= 0 || EQ.Q > 10) 
		{
		err = seterror(2,-1,"Q out of range");
		}	
	}
	
	
	if (EQ.frequency < 20 || EQ.frequency > 20000) 
	{
		err = seterror(2,-1,"frequency filter out of range");
	}

	if (EQ.boost < -80 || EQ.boost > 20) 
	{
		err = seterror(2,-1,"boost out of range");
	}
	
	if (EQ.gain < -80 || EQ.gain > 20) 
	{
		err = seterror(2,-1,"gain out of range");
	}
	
	if(isNot("LPF") && isNot("HPF") && isNot("peak") && isNot("HSF") && isNot("LSF"))
	{
		char* errType = malloc(255*sizeof(char));
		
		sprintf(errType,"%s is unknown type",EQ.type);
		err = seterror(2,-1,errType);
	}
	
	return err;
}

int filtertobiquad			(struct Filter EQ, struct Biquad* biquad)
{
	char* Filt_Descr[5] = {"b0","b1","b2","a1","a2"};
	
	if (strcmp(EQ.type,"peak") == 0)
	{
		double Ax = pow(10,EQ.boost/40);
		double omega = 2 * PI * EQ.frequency / Fs;
		double cs = cos(omega);
		double sn = sin(omega);
		double alpha = sn / (2 * (EQ.Q));
		double a0 =  1 + (alpha / Ax);
		double gainlinear = pow(10,((double)EQ.gain / 20)) / a0;
		
		biquad->value[0] = (1 + (alpha * Ax)) * gainlinear;
		biquad->value[1] =  -( 2 * cs) * gainlinear;
		biquad->value[2] = (1 - (alpha * Ax)) * gainlinear;
		biquad->value[3] =  -( 2 * cs) / a0;
		biquad->value[4] =    (1 - (alpha / Ax)) / a0;

		biquad->value[3] =  biquad->value[3]*(-1);
		biquad->value[4] =  biquad->value[4]*(-1);		

		
		
	}
	if (strcmp(EQ.type,"HPF") == 0)
	{
	/* Chebyshev HPF with constant ripple 0.1 >> Do not match with SigmaStudio results
		double wp = (2 * PI * EQ.frequency) / Fs;
		double Omegap = 1 / tan(wp / 2); 
		double epass = sqrt(	pow(10,(0.1 * chebyshev_ripple)) - 1	);
		double alpha = (0.5) * sqrt( log(1 / epass + (1 / (epass*epass) + 1))	); 
		double Omega0 = Omegap * sinh(alpha); 
		double theta = (PI / 4) * 3;
		double Omega1 = Omegap * sin(theta);
		double H0 = sqrt((1 / (1 + (epass*epass))));
		double Den = 1 - 2 * Omega0 * cos(theta) + (Omega0*Omega0) + (Omega1*Omega1);
		double G = ((Omega0*Omega0) + (Omega1*Omega1)) / Den;
			
		biquad->value[3] = ( - 2 * ((Omega0*Omega0) + (Omega1*Omega1) - 1)) / Den;
		biquad->value[4] = (1 + 2 * Omega0 * cos(theta) + (Omega0*Omega0) + (Omega1*Omega1)) / Den;
		biquad->value[0] = H0 * G * (pow(10,(EQ.gain / 20)));
		biquad->value[1] = - biquad->value[0] * 2;
		biquad->value[2] = biquad->value[0];
	*/

	
	/*Bessel HPF
	*/
	double omega = 2 * PI * EQ.frequency / Fs;
	double sn = sin(omega);
	double cs = cos(omega);
	double alpha = sn / (2 * (1 / sqrt((3))));
	double a0 = (1 + alpha);

	biquad->value[3] = -( 2 * cs) / a0;
	biquad->value[4] = (1 - alpha) / a0;
	biquad->value[1] = -( 1 + cs) / a0 * (pow(10,(EQ.gain / 20)));
	biquad->value[0] = - biquad->value[1] / 2;
	biquad->value[2] = biquad->value[0];
	
	
	biquad->value[3] =  biquad->value[3]*(-1);
	biquad->value[4] =  biquad->value[4]*(-1);	
	
	
	
	}
	/*
		Bessel LPF
	*/
		if (strcmp(EQ.type,"LPF") == 0)
	{
		double omega = 2 * PI * EQ.frequency / Fs;
		double sn = sin(omega);
		double cs = cos(omega);
		double alpha = sn / (2 * (1 / sqrt((3))));
		double a0 = (1 + alpha);

		biquad->value[3] = -( 2 * cs) / a0;									//A1
		biquad->value[4] = (1 - alpha) / a0;								//A2
		biquad->value[1] = ( 1 - cs) / a0 * (pow(10,(EQ.gain / 20)));		//B1
		biquad->value[0] = biquad->value[1] / 2;							//B0
		biquad->value[2] = biquad->value[0];								//B2
		
		biquad->value[3] =  biquad->value[3]*(-1);
		biquad->value[4] =  biquad->value[4]*(-1);
		
		
		
	}

	if (strcmp(EQ.type,"LSF") == 0)
	{
		double w0 = 2 * PI * EQ.frequency / Fs;
		double A = pow(10,(EQ.boost /40)); 
		double alpha = sin(w0)/2 * sqrt( (A+1/A) * (1/EQ.S-1) + 2 );
		//double gamma = sin(w0) * sqrt( (A*A + 1)*(1/EQ.S - 1) + 2*A );
		double gamma = 2*sqrt(A)*alpha;
		
		double a0 = ((A+1) + (A-1)*cos(w0) + gamma);
		double Glin = 1/ a0;
		
		
		biquad->value[0] = A*( (A+1) - (A-1)*cos(w0) + gamma ) / a0;
		biquad->value[1] = 2*A*( (A-1) - (A+1)*cos(w0) ) / a0;
		biquad->value[2] = A*( (A+1) - (A-1)*cos(w0) - gamma ) / a0;
		biquad->value[3] = -2*( (A-1) + (A+1)*cos(w0) ) / a0;
		biquad->value[4] = ((A+1) + (A-1)*cos(w0) - gamma) / a0;
		
		biquad->value[3] =  biquad->value[3]*(-1);
		biquad->value[4] =  biquad->value[4]*(-1);
		
		// printf("\n{B0:%f | B1:%f | B2:%f | A1:%f | A2:%f}",biquad->value[0],biquad->value[1],biquad->value[2],biquad->value[3],biquad->value[4] );

	}
	if (strcmp(EQ.type,"HSF") == 0)
	{
		
	
		double w0 = 2 * PI * EQ.frequency / Fs;
		double A = pow(10,(EQ.boost /40)); 
		double alpha = sin(w0)/2 * sqrt( (A+1/A) * (1/EQ.S-1) + 2 );
		double a0 = ((A+1) - (A-1)*cos(w0) + 2*sqrt(A)*alpha);
		double Glin = 1/a0;
		
		
		biquad->value[0] = A*( (A+1) + (A-1)*cos(w0) + 2*sqrt(A)*alpha ) / a0;
		biquad->value[1]= -2*A*( (A-1) + (A+1)*cos(w0) ) /a0;
		biquad->value[2] = A*( (A+1) + (A-1)*cos(w0) - 2*sqrt(A)*alpha ) / a0 ;
		biquad->value[3] = 2*( (A-1) - (A+1)*cos(w0) ) / a0;
		biquad->value[4] = ((A+1) - (A-1)*cos(w0) - 2*sqrt(A)*alpha) / a0;

		biquad->value[3] =  biquad->value[3]*(-1);
		biquad->value[4] =  biquad->value[4]*(-1);
		
		// printf("\n{B0:%f | B1:%f | B2:%f | A1:%f | A2:%f}",biquad->value[0],biquad->value[1],biquad->value[2],biquad->value[3],biquad->value[4] );

	}
	
	
	if (strcmp(EQ.type,"LPLR") == 0)
	{
		//2nd Order LOWPASS
		double omega = 2 * PI * EQ.frequency / Fs;
		double sn = sin(omega);
		double cs = cos(omega);
		double alpha = sn / (2 * (1 / sqrt(2)));
		double a0 = 1 + alpha;
		double gain = 0;

		biquad->value[3] = -( 2 * cs) / a0;							//A1
		biquad->value[4] = (1 - alpha) / a0;							//A2
		biquad->value[1] = (1 - cs) / a0; 							//B1 			 * (	pow(10,(gain / 20))	) = 1
		biquad->value[0] = biquad->value[1] / 2;						//B0
		biquad->value[2] = biquad->value[0];							//B2
		
		
		float temp = 0;
		int i = 0;
		long int li_bqd=0;
		
		for(i=0;i<5;i++)
		{
			//		complément à 2
			li_bqd = FloatToFixpoint(biquad->value[i]);
			
			if(li_bqd > pow(2,27))
			{
				li_bqd = (li_bqd+1) & 0xFFFFFFF ;
				biquad->value[i] =  FixpointToFloat(li_bqd) * (-1);
			}
					
			
			// need to revert the sign of A1 and A2
			if( i == 3 || i ==4 )
			{
				biquad->value[i] = biquad->value[i] * -1.0;
			}
			
		}
		// DSP loading of crossover is not straightforward as EQ. The order is B0,B1,A1,B2,A2 >> Need to switch A1 & B2

		temp = biquad->value[2];
		biquad->value[2] = biquad->value[3];			// A1 = B2
		biquad->value[3] = temp;
		
		// printf("\n Biquad LPF : {B0 : %f | B1 : %f | A1 : %f | B2 : %f | A2 : %f}",biquad->value[0],biquad->value[1],biquad->value[2],biquad->value[3],biquad->value[4] );

	}
	if (strcmp(EQ.type,"HPLR") == 0)
	{
		//2nd Order HIGHPASS

		double omega = 2 * PI * EQ.frequency / Fs;
		double sn = sin(omega);
		double cs = cos(omega);
		double alpha = sn / (2 * (1 / sqrt(2)));
		double a0 = 1 + alpha;
		double gain = 0;
		
		
		biquad->value[3] = -( 2 * cs) / a0;						// A1
		biquad->value[4] = (1 - alpha) / a0;						// A2
		biquad->value[1] = -(1 + cs) / a0;						// B1	* (	pow(10,(gain / 20))	) = 1
		biquad->value[0] = - biquad->value[1] / 2;				// B0
		biquad->value[2] = biquad->value[0];						// B2

		float temp = 0;
		int i = 0;
		long int li_bqd=0;
		
		for(i=0;i<5;i++)
		{
			//		complément à 2
			li_bqd = FloatToFixpoint(biquad->value[i]);
			
			if(li_bqd > pow(2,27))
			{
				li_bqd = (li_bqd+1) & 0xFFFFFFF ;
				biquad->value[i] =  FixpointToFloat(li_bqd) * (-1);
			}
						
			// need to revert the sign of A1 and A2
			if( i == 3 || i == 4 )
			{
				biquad->value[i] = biquad->value[i] * -1.0;
			}
			
		}
		// DSP loading of crossover is not straightforward as EQ. The order is B0,B1,A1,B2,A2 >> Need to switch A1 & B2

		temp			 = biquad->value[2];
		biquad->value[2] = biquad->value[3];			// A1 = B2
		biquad->value[3] = temp;
		
		// printf("\n Biquad HPF : {B0 : %f | B1 : %f | A1 : %f | B2 : %f | A2 : %f}",biquad->value[0],biquad->value[1],biquad->value[2],biquad->value[3],biquad->value[4]);
	


	}

}

void biquadtosafeload		(struct Biquad biquad, struct Safeload_Data* SLData)
{
	
int j;
// Prepare data to safeload
	for(j=0;j<5;j++)
	{
		
		//assign values
		SLData->SL_data[j] = FloatToFixpoint(biquad.value[j]);
		SLData->SL_address[j] = SLData->start_address + j;	
		
		// printf("\n safeload data %d : 0x%X @reg: %d", j,SLData->SL_data[j], SLData->SL_address[j]);
	}
	
}

f_return createfilters			(char *pConfigFile)
{
	
	int i;
	int j=0;
	f_return err;
	struct Filter EQ[filter_count];
	struct Biquad Bqd[filter_count];
	struct Safeload_Data SLData;
	unsigned long int SL_data[5];
	int SL_reg[5];
	int section_validity = 0 ;
	
	
	char *filterName = malloc(6*sizeof(char));
	
	for(i=1;i<filter_count;i++)
	{
// Load filter value from file		
		
	sprintf(filterName,"EQ%d",i);
		
//Vérifier l'existence de la section
		char* value = malloc(255*sizeof(char));
		f_return result = getSectionParameterValue(pConfigFile,filterName,"Type",value);
			
		if(result.errorlevel != 1)
		{
					
			// récupération des valeurs du fichier de config
			EQ[i].frequency 	= 	getSPVf(pConfigFile,filterName,"F");
			EQ[i].boost 		= 	getSPVf(pConfigFile,filterName,"B");
			EQ[i].gain 			= 	getSPVf(pConfigFile,filterName,"Gain");
			EQ[i].type 			= 	getSPVc(pConfigFile,filterName,"Type");
			
			
			
				if(strcmp(trim(value),"LSF")==0 || strcmp(trim(value),"HSF")==0)
				{
					EQ[i].S 			= 	getSPVf(pConfigFile,filterName,"S");
					// printf("\n%s<S>%f",filterName,EQ[i].S);
				}
				else
				{
					EQ[i].Q 			= 	getSPVf(config_file_path,filterName,"Q");
					// printf("\n%s<Q>%f",filterName,EQ[i].Q);
				}
		

			// Check filter value range	
			err = checkfilterrange(EQ[i]);
		
				if( err.errorstate == -1)
				{	
					//err = seterror(1,-1,"Filter range fails");
					return err;

				}
				else
				{
						
					// Convert filter to Biquad	
					filtertobiquad(EQ[i], &Bqd[i]);
					// Write Safeload
					SLData.start_address = MOD_EQ_ALG0_STAGE0_B0_ADDR + j;
					// printf("\n#<%d>@%d : F=%f | boost=%f | gain=%f | type=%s____",i,SLData.start_address,EQ[i].frequency,EQ[i].boost,EQ[i].gain,EQ[i].type);
					
					biquadtosafeload(Bqd[i],&SLData);
					ADAU1701_SAFELOAD_Write_block(0x68,SLData);
					j=j+5;
				}
				
		}
		
		free(value);
	}
	
}

f_return configure_DSP			(char *pConfigFile)
{
	char* input_type;
	int i;
	int ret=0;
	unsigned char data[4]={0,0,0,0};
	unsigned long int li_data[3];
	unsigned char addr[3]={MOD_SIGNAL_SELECTOR_20_ALG0_STAGE0_STEREOSWITCHNOSLEW_ADDR,
								MOD_SIGNAL_SELECTOR_20_ALG0_STAGE1_STEREOSWITCHNOSLEW_ADDR,
									MOD_SIGNAL_SELECTOR_20_ALG0_STAGE2_STEREOSWITCHNOSLEW_ADDR};
	
	f_return err;
	input_type = getSPVc(pConfigFile,"config","audioinput");
	
	if(strcmp(input_type,"analog")==0)
	{
			li_data[0]=0x00000000;
			li_data[1]=0x00800000;
			li_data[2]=0x00000000;
	}
	else if(strcmp(input_type,"digital")==0)
	{
			li_data[0]=0x00800000;
			li_data[1]=0x00000000;
			li_data[2]=0x00000000;
	}
	else if(strcmp(input_type,"tone")==0)
	{
			li_data[0]=0x00000000;
			li_data[1]=0x00000000;
			li_data[2]=0x00800000;
	}
	else
	{
		err = seterror(1,-1,"unkown input");		
		return err;
	}
	
	for(i=0;i<3;i++)
	{
		Data_Splitter_Table(li_data[i],&data);
		ret = ADAU1701_Write_block(0x68,addr[i],4,data);

	}
	
	if(ret != 0)
		{
			err = seterror(1,-1,"Fail to configure DSP input - data write error");		
			return err;
			
		}
		else
		{
			char* msg = malloc(255*sizeof(char));
			sprintf(msg," input <%s>",input_type);
			err = seterror(0,0,msg);		
			return err;
		}

}	

f_return configure_iochannels	(char *pConfigFile)
{

	int i;
	struct f_return erri;
	struct f_return erro;
	struct channel iChannel[2];
	struct channel oChannel[2];
	unsigned char pData[4]={0,0,0,0};
	unsigned long int ul_volume=0;
	
	//Configure input channel for stereo
	for(i=0;i<2;i++)
	{
		char *sectionName = malloc(8*sizeof(char));
		sprintf(sectionName,"Input%d",i+1);
	
		// Read input channel

			iChannel[i].mute	 = 	getSPVi(pConfigFile,sectionName,"mute");
			iChannel[i].volume	 =	getSPVf(pConfigFile,sectionName,"volume");
			
		
		
		// No phase and delay block on input. Set to the default values for compatibility
			iChannel[i].phase	 = 1;
			iChannel[i].delay	 = 1;
		
		//Configure output channel for Low/High
			
			sprintf(sectionName,"Output%d",i+1);

			oChannel[i].mute	 = 	getSPVi(pConfigFile,sectionName,"mute");
			oChannel[i].volume	 = 	getSPVf(pConfigFile,sectionName,"volume");
			oChannel[i].phase	 = 	getSPVi(pConfigFile,sectionName,"phase");
			oChannel[i].delay	 = 	getSPVi(pConfigFile,sectionName,"delay");
						
			erri = checkchannelconfig(iChannel[i]);
			erro = checkchannelconfig(oChannel[i]);
			
			if(erri.errorstate ==-1)
			{
			return erri;
				
			}
			if(erro.errorstate == -1)
			{
			return erro;
			}

	}

	for(i=0;i<2;i++)
	{
		//Write volume for input
			Data_Splitter_Table(dBtoDec(iChannel[i].volume), pData);
			ADAU1701_Write_block(DSP_Address,MOD_MASTER_20_VOLUME_GAIN1940ALGNS1_ADDR,sizeof(pData),pData);
			
		//Write volume for output
			unsigned long int temp = dBtoDec(oChannel[i].volume);
			Data_Splitter_Table(temp, pData);
				if(i==0)
				{
					ADAU1701_Write_block(DSP_Address,MOD_TREBLE_20_VOLUME_GAIN1940ALGNS3_ADDR,sizeof(pData),pData);
					
				}
				if(i==1)
				{
					
					ADAU1701_Write_block(DSP_Address,MOD_BASS_20_VOLUME_GAIN1940ALGNS4_ADDR,sizeof(pData),pData);
				}
		//Write mute input
			temp = iChannel[i].mute*pow(2,23); 
			Data_Splitter_Table(temp, pData);
			
				if(i==0)
				{
					ADAU1701_Write_block(DSP_Address,MOD_MUTER_MUTENOSLEWALG7MUTE_ADDR,sizeof(pData),pData);					
				}
				if(i==1)
				{
					ADAU1701_Write_block(DSP_Address,MOD_MUTEL_MUTENOSLEWALG6MUTE_ADDR,sizeof(pData),pData);			
				}	
		//Write mute output
			temp = oChannel[i].mute*pow(2,23);
			Data_Splitter_Table(temp, pData);
				if(i==0)
				{
					ADAU1701_Write_block(DSP_Address,MOD_TREBLE_20_MUTE_MUTENOSLEWALG11MUTE_ADDR,sizeof(pData),pData);
					
				}
				if(i==1)
				{
					ADAU1701_Write_block(DSP_Address,MOD_BASS_20_MUTE_MUTENOSLEWALG3MUTE_ADDR,sizeof(pData),pData);
					
				}
		//Write Phase
		//Select a default phase if the value in the config file is wrong (ex: 0)
		
				if(oChannel[i].phase != -1 && oChannel[i].phase != 1)
				{
					oChannel[i].phase = 1;
				}
				if(oChannel[i].phase == 1)
				{
					temp = oChannel[i].phase*pow(2,23);
				}
				else
				{
					temp = 0xFF800000;
				}
			
			Data_Splitter_Table(temp, pData);
				if(i==0)
				{
					ADAU1701_Write_block(DSP_Address,MOD_TREBLE_20_PHASE_EQ1940INVERT1GAIN_ADDR,sizeof(pData),pData);
				}
				if(i==1)
				{
					ADAU1701_Write_block(DSP_Address,MOD_BASS_20_PHASE_EQ1940INVERT2GAIN_ADDR,sizeof(pData),pData);
					
				}
		//Write Delay
			temp = oChannel[i].delay;
			Data_Splitter_Table(temp, pData);
				if(i==0)
				{
					 
					ADAU1701_Write_block(DSP_Address,MOD_TREBLE_20_SAMPLEALIGNER_DELAYAMT_ADDR,sizeof(pData),pData);
				}
				if(i==1)
				{
					ADAU1701_Write_block(DSP_Address,MOD_BASS_20_SAMPLEALIGNER_DELAYAMT_ADDR,sizeof(pData),pData);
				}
	}

}

f_return createcrossover			(char *pConfigFile)
{

	
	struct Biquad Bqd[2];
	struct Safeload_Data SLData[4];
	struct Filter LP_FILTER;
	struct Filter HP_FILTER;

	f_return err;
	
	char* value = malloc(255*sizeof(char));
	err = getSectionParameterValue(pConfigFile,"crossover","FLow",value);
	
	if(err.errorstate == 0)
	{
		LP_FILTER.frequency		=	 getSPVi(pConfigFile,"crossover","FLow");
		LP_FILTER.type = "LPLR";
	}
	else
	{
		return err;
	}
	
	err = getSectionParameterValue(pConfigFile,"crossover","FHigh",value);
	

	  
	if(err.errorstate == 0)
	{
		HP_FILTER.frequency		=	 getSPVi(pConfigFile,"crossover","FHigh");
		HP_FILTER.type = "HPLR";	
		
	}
	else
	{
		return err;
	}
	

	if (LP_FILTER.frequency	< 20 || LP_FILTER.frequency	 > 20000 || HP_FILTER.frequency	 < 20 || HP_FILTER.frequency > 20000) 
	{
		err = seterror(1,-1," frequency filter out of range");
		return err;
	}
	if (LP_FILTER.frequency	 > HP_FILTER.frequency	)
	{
		err = seterror(1,-1,"wrong filter match");
		return err;
	}
	else
	{

// Create LOW & High Pass biquad		
		
		filtertobiquad(LP_FILTER, &Bqd[0]);
		filtertobiquad(HP_FILTER, &Bqd[1]);	
		
		// Write Safeload Low Pass
		SLData[0].start_address = MOD_CROSSOVER_20_ALG0_LOW_FILT1_PARAMB0_ADDR;
		biquadtosafeload(Bqd[0],&SLData[0]);
		ADAU1701_SAFELOAD_Write_block(0x68,SLData[0]);
		
		SLData[1].start_address = MOD_CROSSOVER_20_ALG0_LOW_FILT2_PARAMB0_ADDR;
		biquadtosafeload(Bqd[0],&SLData[1]);
		ADAU1701_SAFELOAD_Write_block(0x68,SLData[1]);
		
		

		// Write Safeload High Pass
		SLData[2].start_address = MOD_CROSSOVER_20_ALG0_HIGH_FILT1_PARAMB0_ADDR;
		biquadtosafeload(Bqd[1],&SLData[2]);
		ADAU1701_SAFELOAD_Write_block(0x68,SLData[2]);
		
		SLData[3].start_address = MOD_CROSSOVER_20_ALG0_HIGH_FILT2_PARAMB0_ADDR;
		biquadtosafeload(Bqd[1],&SLData[3]);
		ADAU1701_SAFELOAD_Write_block(0x68,SLData[3]);

		err = seterror(0,0,"OK");
		return err;

	}	
}



