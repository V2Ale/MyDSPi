#ifndef __DSPDEFINE_H__
#define __DSPDEFINE_H__

#define DSP_Address 			0x68
#define filter_count			12


struct Biquad
	{
		int addr[5];
		float value[5];
		char* description[5];

	};

struct Filter
{
	float frequency;
	float boost;
	float S;
	float Q;
	float gain;
	char* type;
	
};	

struct Crossover
	{
		float LPF_Frequency;
		float LPF_Gain;
		float HPF_Frequency;
		float HPF_Gain;
	};

struct Safeload_Data
	{	
		unsigned long int SL_data[5];
		int SL_address[5];
		int start_address;
		
	};	
	
struct channel
{
	char* ID;
	char* alias;
	int mute;
	float volume;
	int phase;
	int delay;
	
};
	


	
#endif