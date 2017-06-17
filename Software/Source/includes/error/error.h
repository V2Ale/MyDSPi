#ifndef __ERROR_H__
#define __ERROR_H__

typedef struct f_return
{
	int errorstate;
	int errorcode;
	char* errormessage;
	int errorlevel;
}f_return;
	
f_return seterror(int level, int state, char* message);

#endif
