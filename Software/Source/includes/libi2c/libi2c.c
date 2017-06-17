#include <linux/i2c-dev.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <time.h>

//error
#include "../error/error.h"

/*********************************************************************
* i2c_funcs.c : I2C Access Functions
*********************************************************************/

int i2c_fd = -1;				/* Device node: /dev/i2c-1 */
unsigned long i2c_funcs = 0;	/* Support flags */


/**************************************************
 * Open I2C bus and check capabilities :
 */
f_return i2c_init(const char *node) {
	int rc;
	f_return err;
	
	i2c_fd = open(node,O_RDWR);	/* Open driver /dev/i2s-1 */
	if ( i2c_fd < 0 ) {
		perror("Opening /dev/i2s-1");
		puts("Check that the i2c-dev & i2c-bcm2708 kernel modules "
		     "are loaded.");
		err = seterror(1,-1,"Failed to open I2C bus");
		return err;
		abort();
	}

	/*
	 * Make sure the driver supports plain I2C I/O:
	 */
	rc = ioctl(i2c_fd,I2C_FUNCS,&i2c_funcs);

	err = seterror(0,0,"I2C bus openned with success");
	return err;
	
//	assert(rc >= 0);
//	assert(i2c_funcs & I2C_FUNC_I2C);
}

/*
 * Close the I2C driver :
 */
void i2c_close(void) {
	close(i2c_fd);
	i2c_fd = -1;
}


int scan_bus()
{

	int i=0;
	int n=0;
	int ret;
printf("\n**** Scan for I2C devices ****");	
for(i=0;i<256;i++)
	{
		
		ret = ADAU1701_Read_register(i,0); 			// A corriger en utilisant une fonction I2C générique
		if(ret>-1)
		{
			n++;
			printf("\n Device found at address : 0x%X",i);
		}

	}
if(i==0)
{
	printf("\n No device found ");
}
printf("\n**** End of Scan for I2C devices ****\n");	
return n;
}



