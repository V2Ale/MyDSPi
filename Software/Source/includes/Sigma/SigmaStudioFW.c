#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <time.h>

//I2C
#include "../libi2c/libi2c.h"


typedef unsigned short ADI_DATA_U16;
typedef unsigned char  ADI_REG_TYPE;



int SIGMA_WRITE_REGISTER_BLOCK(int devAddress,int address,int length,ADI_REG_TYPE *pData ) 
{
	ADAU1701_Write_block(devAddress,address,length,pData );
} 
