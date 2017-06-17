#ifndef __DSPCrossover_H__
#define __DSPCrossover_H__

//variables


//Prototypes

void ADAU1701_getCrossoverParameters(float* LPF_Frequency,float* LPF_Gain, float* HPF_Frequency, float* HPF_Gain);
void ADAU1701_setCrossoverParameters(float LPF_Frequency,float LPF_Gain, float HPF_Frequency, float HPF_Gain);
void ADAU1701_getEQParameters(struct Biquad *BiquadEQ[16]);

//void EQ_resolveBiquad(struct Biquad EQ_biquad, struct* EQPoint EQ);
void naissance(personne *toto);
#endif