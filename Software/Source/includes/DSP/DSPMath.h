#ifndef __DSPMath_H__
#define __DSPMath_H__

//functions


float DectodB(unsigned long int decimaldigit);
unsigned long int dBtoDec(float dB_value);


long int FloatToFixpoint(double floatingdigit);
float FixpointToFloat(unsigned long int FixedPointDigit);
float roundfnd(float number, int digit);

#endif