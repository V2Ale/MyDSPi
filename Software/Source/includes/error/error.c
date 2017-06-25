#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <sys/types.h>

//Error
#include "error.h"

f_return seterror(int level, int state, char* message)
{
	
	struct f_return err;
	
	err.errormessage = malloc(255*sizeof(char));
	err.errorstate = 0;
	err.errorlevel=0;
	
	err.errorstate = state;
	err.errorlevel = level;
	
	sprintf(err.errormessage, message);
	
	return err;
}