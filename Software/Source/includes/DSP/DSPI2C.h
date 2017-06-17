#ifndef __DSPI2C_H__
#define __DSPI2C_H__

long int ADAU1701_Read_register(int addr,unsigned int reg);
int ADAU1701_Write_block(int devAddress,int address,int length,unsigned char *pData );
long int ADAU1701_Read_register(int addr, unsigned int reg);
int ADAU1701_Read_ConfigRegister(int addr, unsigned int reg);

#endif