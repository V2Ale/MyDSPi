
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <time.h>
#include <math.h>

//error
#include "../error/error.h"
/* I2C Librairies include */
#include "../libi2c/libi2c.h"
#include <linux/i2c-dev.h>

#include "DSPDefine.h"


/**************************************************
 * WRITE_REGISTER_BLOCK
 */

int ADAU1701_Write_block(int devAddress,int address,int length,unsigned char *pData ) 
{
	struct i2c_rdwr_ioctl_data msgset;
	struct i2c_msg iomsgs[1];
	char buf[length+2];
	int rc;
	int i;
	
	buf[0] = (char) (( address >> 8 ) & 0xFF);
	buf[1] = (char) (address & 0xFF);

	
	for(i=2;i<length+2;i++)
	{
		buf[i] = (char) pData[i-2];	
	}
	
	devAddress = (devAddress >> 1);
	
	iomsgs[0].addr = (unsigned) devAddress;
	iomsgs[0].flags = 0;		
	iomsgs[0].buf = buf;
	iomsgs[0].len = length + 2;	// 2 bytes for address

	msgset.msgs = iomsgs;
	msgset.nmsgs = 1;

	rc = ioctl(i2c_fd,I2C_RDWR,&msgset);
	if (rc!=-1)
	{
	/// To Do : écriture dans le log	
	// printf("\nDSP:0%X at R:%d write %d registers : OK", (devAddress<<1),address,length);
	}
	else
	{
		perror("Failed to write DSP register");
	/// To Do : écriture dans le log	
	//	printf("\nDSP:0%X at Reeg %d write %d registers : OK", (devAddress<<1),address,length);
	}
	
	return rc < 0 ? -1 : 0;
} 

int ADAU1701_SAFELOAD_Write_block(int devAddress,struct Safeload_Data SLD) 
{
	//Write the 5 blocks of Data
	struct i2c_rdwr_ioctl_data msgset;
	struct i2c_msg iomsgs[1];
	struct i2c_msg addr_msgs[1];
	char buf[10];
	int rc; 
	int i;
	int j;
	unsigned long int SAFELOAD_ADDRESS_REGISTERS[5] 	= {0x0815,0x0816,0x0817,0x0818,0x0819};
	unsigned long int SAFELOAD_DATA_REGISTERS[5] 		= {0x0810,0x0811,0x0812,0x0813,0x0814};
	unsigned long int SAFELOAD_ADDRESS_TRANSFERT 		= 0x081C; 
	int SL_Addr_state = 0;
	int SL_Data_state = 0;
	
	// 
	devAddress = (devAddress >> 1);
	
	for(i=0;i<5;i++)
	{
	//Write data block
		//buf0,1 : Addresss
		//Buff2,3,4,5 : Data
		
		buf[0] = (char) ((  SAFELOAD_DATA_REGISTERS[i] >> 8 ) & 0xFF);
		buf[1] = (char) ( SAFELOAD_DATA_REGISTERS[i] & 0xFF);
		buf[2] = 0x00;
		unsigned char temp[4]={0,0,0,0};
		Data_Splitter_Table(SLD.SL_data[i],&temp);
		
		
		
		for(j=3;j<8;j++)
		{			
			buf[j] = (char) temp[j-3];	
		}	
			// printf("\n SL.data: buf[0]=0x%X - buf[1]=0x%X - buf[2]=0x%X - buf[3]=0x%X - buf[4]=0x%X - buf[5]=0x%X - buf[6]=0x%X", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6]);
		
			
			
			iomsgs[0].addr = (unsigned) devAddress;
			iomsgs[0].flags = 0;		
			iomsgs[0].buf = buf;
			iomsgs[0].len = 7;	// 2 bytes for address

			msgset.msgs = iomsgs;
			msgset.nmsgs = 1;

			rc = ioctl(i2c_fd,I2C_RDWR,&msgset);
			// usleep(100);
				if (rc!=-1)
				{
				SL_Data_state = 1;
	/// To Do : écriture dans le log	
				//	printf("\nDSP:0%X at Reeg %d write %d registers : OK", (devAddress<<1),address,length);
				}
				else
				{
					perror("Failed to write DSP register");
	/// To Do : écriture dans le log	
				//	printf("\nDSP:0%X at Reeg %d write %d registers : OK", (devAddress<<1),address,length);
				return -1;
				}
	
		
	//Write the 5 blocks of Addresss
		
		buf[0] = (char) (( SAFELOAD_ADDRESS_REGISTERS[i] >> 8 ) & 0xFF);
		buf[1] = (char) (SAFELOAD_ADDRESS_REGISTERS[i] & 0xFF);
		buf[2] = 0x00;
		buf[3] = (char) SLD.SL_address[i];																				
		// printf("\n SL.addr: buf[0]=0x%X - buf[1]=0x%X - buf[2]=0x%X - buf[3]=0x%X", buf[0], buf[1], buf[2],  buf[3]);

			//devAddress = (devAddress >> 1);
			
			addr_msgs[0].addr = (unsigned) devAddress;
			addr_msgs[0].flags = 0;		
			addr_msgs[0].buf = buf;
			addr_msgs[0].len = 4;	

			msgset.msgs = addr_msgs;
			msgset.nmsgs = 1;

			rc = ioctl(i2c_fd,I2C_RDWR,&msgset);
			// usleep(100);
				if (rc!=-1)
				{
				SL_Addr_state = 1;
	/// To Do : écriture dans le log	
				//	printf("\nDSP:0%X at Reeg %d write %d registers : OK", (devAddress<<1),address,length);
				}
				else
				{
					perror("Failed to write DSP register");
					return -1;
	/// To Do : écriture dans le log	
				//	printf("\nDSP:0%X at Reeg %d write %d registers : OK", (devAddress<<1),address,length);
				}	
		// usleep(2000);
	}
	
		
	
	//Init transfert
	if(SL_Addr_state ==1 && SL_Data_state ==1)
	{
		buf[0] = (char) (( SAFELOAD_ADDRESS_TRANSFERT >> 8 ) & 0xFF);
		buf[1] = (char) (SAFELOAD_ADDRESS_TRANSFERT & 0xFF);
		buf[2] = 0x00;	
		buf[3] = 0x3C;	
		
		// printf("\n SL Push write :\n buf[0]=0x%X buf[1]=0x%X buf[2]=0x%X buf[3]=0x%X", buf[0], buf[1], buf[2], buf[3]);

			// devAddress = (devAddress >> 1);
			
			iomsgs[0].addr = (unsigned) devAddress;
			iomsgs[0].flags = 0;		
			iomsgs[0].buf = buf;
			iomsgs[0].len = 4;	

			msgset.msgs = iomsgs;
			msgset.nmsgs = 1;

			rc = ioctl(i2c_fd,I2C_RDWR,&msgset);
			if (rc!=-1)
			{
			SL_Addr_state = 2;
			SL_Data_state = 2;
			/// To Do : écriture dans le log	
			//	printf("\nDSP:0%X at Reeg %d write %d registers : OK", (devAddress<<1),address,length);
			}
			else
			{
				perror("Failed to write DSP register");
			/// To Do : écriture dans le log	
			//	printf("\nDSP:0%X at Reeg %d write %d registers : OK", (devAddress<<1),address,length);
			}
			
			return rc < 0 ? -1 : 0;		

	}
	

}

/**************************************************
 * Read one registrer from DSP ADAU1701
 */
 
 // former function name i2c_read_dsp
long int ADAU1701_Read_register(int addr, unsigned int reg) 
{
	struct i2c_rdwr_ioctl_data msgset;
	struct i2c_msg iomsgs[2];
	
	char buf[2];
	char rbuf[4];
	int rc;

	
    buf[0] =  (char)(( reg >> 8 ) & 0xFF);
	buf[1] =  (char)(reg & 0xFF);
	/*
	printf("buf[1]=%d\n",buf[1]);
	printf("buf[0]=%d\n",buf[0]);
	*/
	

	addr = addr >> 1;	
	
	
	
	iomsgs[0].addr = iomsgs[1].addr = (unsigned) (addr);
	iomsgs[0].flags = 0;				/* Write */ 
	iomsgs[0].buf = buf;
	iomsgs[0].len = 2;										

	iomsgs[1].flags = I2C_M_RD;					// I2C_M_NOSTART;			//I2C_M_RD;
	iomsgs[1].buf = rbuf;				
	iomsgs[1].len = 4;					

	msgset.msgs = iomsgs;
	msgset.nmsgs = 2;

	
		if ( (rc = ioctl(i2c_fd,I2C_RDWR,&msgset)) < 0 )
		{
		perror("Failed to read DSP register");
			/// To Do : écriture dans le log	
			//	printf("\nDSP:0%X at Reeg %d write %d registers : OK", (devAddress<<1),address,length);	
			return -1;
		}
		
	return (long int)(rbuf[0] << 24) | (rbuf[1] << 16) | (rbuf[2] << 8) | (rbuf[3] << 0);

}

int ADAU1701_Read_ConfigRegister(int addr, unsigned int reg) 
{
	struct i2c_rdwr_ioctl_data msgset;
	struct i2c_msg iomsgs[2];
	
	char buf[2];
	char rbuf[4];
	int rc;

	
    buf[0] =  (char)(( reg >> 8 ) & 0xFF);
	buf[1] =  (char)(reg & 0xFF);
	/*
	printf("buf[1]=%d\n",buf[1]);
	printf("buf[0]=%d\n",buf[0]);
	*/
	

	addr = addr >> 1;	
	
	
	
	iomsgs[0].addr = iomsgs[1].addr = (unsigned) (addr);
	iomsgs[0].flags = 0;				/* Write */ 
	iomsgs[0].buf = buf;
	iomsgs[0].len = 2;										

	iomsgs[1].flags = I2C_M_RD;					// I2C_M_NOSTART;			//I2C_M_RD;
	iomsgs[1].buf = rbuf;				
	iomsgs[1].len = 2;					

	msgset.msgs = iomsgs;
	msgset.nmsgs = 2;

	
		if ( (rc = ioctl(i2c_fd,I2C_RDWR,&msgset)) < 0 )
		{
		perror("Failed to read DSP register");
			/// To Do : écriture dans le log	
			//	printf("\nDSP:0%X at Reeg %d write %d registers : OK", (devAddress<<1),address,length);	
			return -1;
		}
		
	return (int)(rbuf[2] << 8) | (rbuf[3] << 0);

}
