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

#define PI 3.141592654 //3.141592653589793

/* Source : https://wiki.analog.com/resources/tools-software/sigmastudio/usingsigmastudio/numericformats
* 5.23 (decimal) : A full scale 24-bit input signal would have a positive peak of 1000 0000 0000 0000 0000 0000 in unsigned, 24-bit representation
Moving the leading '1' to the left or right will double or halve the value, respectively.
0000 0000 0000 0000 0000 0000 0000 = 0.0
0000 0010 0000 0000 0000 0000 0000 = 0.25
0000 1000 0000 0000 0000 0000 0000 = 1.0 (0 dB full scale)
* 28.0 (integer) : 
0000 0000 0000 0000 0000 0000 0000 = 0
0000 0000 0000 0000 0000 0000 0001 = 1
...
0000 1000 0000 0000 0000 0000 0000 = 8388608 (0 dB full scale represented in 28.0 format)
...
0111 1111 1111 1111 1111 1111 1111 = 134217727 (2^27 - 1)
Negative numbers are represented in two's complement
*/

/* Convert dec number in 5.23 into decimal dB number
 */
 float DectodB(unsigned long int decimaldigit)
 {
	float temp = 0.0;
 
	temp = decimaldigit * pow(2,-23);
	temp = 20.0 * log10(temp);
	temp = round(temp);
	
	return temp;

 }
 
/* Convert dB number in decimal 5.23
 */
unsigned long int dBtoDec(float dB_value)
 {
	float temp = 0.0;
	unsigned long int return_value;
	temp = pow(10,dB_value/20);
	
	temp = temp * pow(2,23);
	return_value = (unsigned long int) round(temp);
	
	return return_value;

 }
 
 
 /*********************************************************************
* Data_Splitter_Table(unsigned long int msg, char *pData)
transform the msg into a table of char and return it though argument pData
Return 0 for fail / 1 for OK
ex : msg = 420426 (0x66A4A) >> pData[0] = 00 pData[1] = 06 pData[2] = 6A pData[3] = 4A   
*********************************************************************/

int Data_Splitter_Table(unsigned long int msg,unsigned char *pData)
{
	int j = sizeof(msg);
	int i;
	
	for(i=0;i<j;i++)
	{
		pData[j-i-1] = (unsigned char) (msg >> i*8) & 0xFF;
		//printf("  0x%X", pData[j-i-1]);
	}
}

/*	Convert a float number into 28.0 fix point number
*
*/
long int FloatToFixpoint(double floatingdigit)
{
	
	return (long int)(floatingdigit*8388608);
}

/*	Convert a 28.0 fix point number into 5.23 floating point number
*
*/
float FixpointToFloat(unsigned long int FixedPointDigit)
{
	
	
	return (float)(FixedPointDigit*pow(2,-23));
}

/*	Round a number 'number' with a precision of 'digit' digits
*
*/
float roundfnd(float number, int digit)
{
	int N = pow(10,digit);
	if(digit > 0)
	{
		return roundf(( number *  N )/ N);
	}
	else
	{
///	Retour d'erreur
	}
	
}
