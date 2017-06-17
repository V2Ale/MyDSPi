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

//Error
#include "includes/error/error.h"

// Configuration
#include "config.h"

//I2C
#include <linux/i2c-dev.h>
#include "includes/libi2c/libi2c.h"

// DSP
#include "includes/Sigma/SS_MyDSPi_V122_IC_1.h"
// #include "includes/Sigma/PP_MyDSPi_V120_IC_1.h"



#include "includes/DSP/DSPI2C.h"
#include "includes/DSP/DSPMath.h"

#include "includes/DSP/DSPDefine.h"
#include "includes/DSP/DSPBiquad.h"
#include "includes/Sigma/SigmaStudioFW.c"

//Ini manager

#include "includes/libini/libini.h"


#define PE_Address 0x40
#define DSP_Address 0x68

#define Master_Volume_Addr0 56
#define Master_Volume_Addr1 57
typedef unsigned long int UINT32;

#define PI 						3.141592654
#define Fs 						48000
#define MOD_CROSSOVER_20_ALG0_LOW_FILT1_PARAMB0_ADDR 90


void main(int argc, char *argv[])
{

struct f_return ret[5];
f_return i2cstatus;
f_return globalerr;

printf("\n**********START Configuration of DSP *************\n");

i2cstatus = i2c_init("/dev/i2c-1");
	if(i2cstatus.errorstate !=-1)
	{
		//check device on i2c bus
		int i2cstatus = ADAU1701_Read_register(0x68,0x081C); 			
		if(i2cstatus>-1)
		{
		//Init DSP default values
		default_download_IC_1();


		int DSP_CORE_REG_VALUE = ADAU1701_Read_ConfigRegister(DSP_Address,0x081E);
		printf("\n DSP CORE : 0x%X",DSP_CORE_REG_VALUE);
		// ADAU1701_Write_block(DSP_Address,0x081E,1,0);

		/// To do: Mute all outputs and inputs
		//  

		// Check config file exist	
			FILE *fp;
						
			fp = fopen(config_file_path,"r");
			
			if(fp == NULL)
			{
				globalerr = seterror(1,-1,"config file not found");
				exit(EXIT_FAILURE);
			}
		/// to do : close config file

			ret[0] = configure_DSP(config_file_path);
			if(ret[0].errorstate!=-1)
			{
				printf("\n DSP configuration : OK >> %s", ret[0].errormessage);
			}
			else
			{
				printf("\n DSP configuration : Fail >> %s", ret[0].errormessage);
			}

			ret[1] = configure_iochannels(config_file_path);
			if(ret[1].errorstate!=-1)
			{
				printf("\n I/O configuration : OK");
			}
			else
			{
				printf("\n I/O configuration : Fail >> %s", ret[1].errormessage);
			}
			


			ret[3] = createcrossover(config_file_path);
			if(ret[3].errorstate!=-1)
			{
				printf("\n Crossover configuration : OK");
			}
			else
			{
				printf("\n Crossover configuration : Fail >> %s",ret[3].errormessage);
			}

			

			
			ret[2] = createfilters(config_file_path);
			if(ret[2].errorstate!=-1)
			{
				printf("\n EQ configuration : OK");
			}
			else
			{
				printf("\n EQ configuration : Fail >> %s", ret[2].errormessage);
			}
		}



	}



printf("\n**********END Configuration of DSP *************\n");



}

	

		 
	



	




