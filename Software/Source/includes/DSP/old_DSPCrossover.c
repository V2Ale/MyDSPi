#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <sys/types.h>

#include <math.h>
#include "DSPDefine.h"
#include "DSPMath.h"
#include "DSPI2C.h"

//I2C
#include <linux/i2c-dev.h>
#include "../libi2c/libi2c.h"


#define PI 3.141592654
#define Fs 48000.0

// LPF Biquad
struct Biquad Crossover_Biquad1;
struct Biquad Crossover_Biquad2;
// HPF Biquad
struct Biquad Crossover_Biquad3;
struct Biquad Crossover_Biquad4;





void ADAU1701_getCrossoverParameters(float* LPF_Frequency,float* LPF_Gain, float* HPF_Frequency, float* HPF_Gain)
{

	char* Crossover_Filt_Descr[5] = {"A1","A2","B0","B1","B2"};
	int i;
	// LPF Filters
		const int Crossover_Filt1_Addr[5] = {62,64,60,61,63};
		const int Crossover_Filt2_Addr[5] = {67,69,65,66,68};
	
	// HPF Filters
		const int Crossover_Filt3_Addr[5] = {72,74,70,71,73};
		const int Crossover_Filt4_Addr[5] = {77,79,75,76,78};
	
	for(i=0;i<5;i++)
	{
		//Initialisation des adresses
		Crossover_Biquad1.Addr[i] = Crossover_Filt1_Addr[i];
		Crossover_Biquad2.Addr[i] = Crossover_Filt2_Addr[i];
		Crossover_Biquad3.Addr[i] = Crossover_Filt3_Addr[i];
		Crossover_Biquad4.Addr[i] = Crossover_Filt4_Addr[i];
		//Initialisation des descripteurs
		Crossover_Biquad1.Description[i] = Crossover_Filt_Descr[i];
		Crossover_Biquad2.Description[i] = Crossover_Filt_Descr[i];
		Crossover_Biquad3.Description[i] = Crossover_Filt_Descr[i];
		Crossover_Biquad4.Description[i] = Crossover_Filt_Descr[i];
		//Récupération des valeurs des biquads
		Crossover_Biquad1.Value[i] = ADAU1701_Read_register(DSP_Address,Crossover_Biquad1.Addr[i]);
		Crossover_Biquad2.Value[i] = ADAU1701_Read_register(DSP_Address,Crossover_Biquad2.Addr[i]);
		Crossover_Biquad3.Value[i] = ADAU1701_Read_register(DSP_Address,Crossover_Biquad3.Addr[i]);
		Crossover_Biquad4.Value[i] = ADAU1701_Read_register(DSP_Address,Crossover_Biquad4.Addr[i]);
		// Complément à 2
		if(Crossover_Biquad1.Value[i] > pow(2,27))
		{
			
			Crossover_Biquad1.Value[i] =  (~Crossover_Biquad1.Value[i] + 1) & 0xFFFFFFF;
		
		}
		if(Crossover_Biquad2.Value[i] > pow(2,27))
		{
			
			Crossover_Biquad2.Value[i] =  (~Crossover_Biquad2.Value[i] + 1) & 0xFFFFFFF;
		
		}
		if(Crossover_Biquad3.Value[i] > pow(2,27))
		{
			
			Crossover_Biquad3.Value[i] =  (~Crossover_Biquad3.Value[i] + 1) & 0xFFFFFFF;
		
		}
		if(Crossover_Biquad4.Value[i] > pow(2,27))
		{
			
			Crossover_Biquad4.Value[i] =  (~Crossover_Biquad4.Value[i] + 1) & 0xFFFFFFF;
		
		}
		
	}
	
	
		// LPF_Frequency = (FS * Omega) / (2 * PI)

		float LPF_w;
		float LPF_alpha;
		float LPF_Q = 0.707;															// SQRT(2)/2
		float LPF_A2 = FixpointToFloat(Crossover_Biquad1.Value[1]) * (-1);				//Inversion de A2 lié à une inversion SigmaDSP volontaire de la part d'Analog Device
		float LPF_A1 = FixpointToFloat(Crossover_Biquad1.Value[0]) * (-1);				//Inversion de A2 lié à une inversion SigmaDSP volontaire de la part d'Analog Device
		float LPF_B1 = FixpointToFloat(Crossover_Biquad1.Value[3]);

		LPF_alpha = -(1 + LPF_A2) / (LPF_A2 - 1);
		LPF_w = asin((double)2*LPF_alpha*LPF_Q);
		*LPF_Frequency = LPF_w * Fs / (2*PI);
		*LPF_Gain = 20 * log10((1+LPF_alpha)*LPF_B1/(1 - cos(LPF_w)));

		// HPF_Frequency = (FS * Omega) / (2 * PI)

		float HPF_w;
		float HPF_alpha;
		float HPF_Q = 0.707;															// SQRT(2)/2
		float HPF_A2 = FixpointToFloat(Crossover_Biquad3.Value[1]) * (-1);				//Inversion de A2 lié à une inversion SigmaDSP volontaire de la part d'Analog Device
		float HPF_A1 = FixpointToFloat(Crossover_Biquad3.Value[0]) * (-1);				//Inversion de A2 lié à une inversion SigmaDSP volontaire de la part d'Analog Device
		float HPF_B1 = FixpointToFloat(Crossover_Biquad3.Value[3]);	

		HPF_alpha = -(1 + HPF_A2) / (HPF_A2 - 1);
		HPF_w = asin((double)2*HPF_alpha*HPF_Q);
		*HPF_Frequency = HPF_w * Fs / (2*PI);
		*HPF_Gain = 20*log10(((1+HPF_alpha)*HPF_B1)/(1 + cos(HPF_w)));
		
}

void ADAU1701_setCrossoverParameters(float LPF_Frequency,float LPF_Gain, float HPF_Frequency, float HPF_Gain)
{
//******************    To DO	 *********************
	
// Vérification si fréquence et gain ne sont pas hors range

// Lecture et sauvegarde des biquads actuels

// Calcul des biquads & écriture dans une structure

// Activation du safeload

// Envoi des biquads

// Relecture et vérification


}

void ADAU1701_getEQParameters(struct Biquad *BiquadEQ[16])
{
	// Init biquad struct
	int i=0;
	int j=0;
	int I2C_EQ_StartAddr[16] = {4,9,14,19,24,29,34,39,44,49,80,85,90,95,100,105};
	char* Crossover_Filt_Descr[5] = {"A1","A2","B0","B1","B2"};
	
	
	// Initialisation des addresses des biquads
	// Toutes les addresses sont consécutives en mémoire
	// Puis lecture de tous les biquads
	
	for(i=0;i<16;i++)
	{	
		//printf("Biquad n° %d",i);
		for(j=0;j<5;j++)
		{
			BiquadEQ[i]->Description[j] = Crossover_Filt_Descr[j];
			BiquadEQ[i]->Addr[j] = I2C_EQ_StartAddr[i] + j ;
			BiquadEQ[i]->Value[j] = ADAU1701_Read_register(DSP_Address,BiquadEQ[i]->Addr[j]);
				// Complément à 2
				if(BiquadEQ[i]->Value[j] > pow(2,27))
				{
					
					BiquadEQ[i]->Value[j] =  (~BiquadEQ[i]->Value[j] + 1) & 0xFFFFFFF;
				
				}
			//printf("\n    Adresse :  %d \n %s: %f",BiquadEQ[i].Addr[j],BiquadEQ[i].Description[j],FixpointToFloat(BiquadEQ[i].Value[j]));
		}	
	}
	
	
}

/*	Function that have a biquad in input and output the EQ parameters in user understandable format
*	Ex :  input EQ_biquad with B0=1,334576, B1=0,923234 etc... >> output EQ frequency = 234, Q=0,707 etc...


void EQ_resolveBiquad(struct Biquad EQ_biquad, struct* EQPoint EQ)
{
	
}
*/

void naissance(personne *toto)
{
	toto->nom="Kevin";
	toto->age=15;
}