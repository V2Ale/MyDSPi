#ifndef __DSPBIQUAD_H__
#define __DSPBIQUAD_H__


void 	ADAU1701_ReadBiquad		(int DSPAddress,int register_address,struct Biquad* biquad);
f_return	createcrossover			(char *pConfigFile);
void	biquadtosafeload		(struct Biquad biquad, struct Safeload_Data* SLData);
int 	filtertobiquad			(struct Filter EQ, struct Biquad* biquad);
f_return 	createfilters			(char *pConfigFile);
f_return	checkfilterrange		(struct Filter EQ);

f_return	configure_DSP			(char *pConfigFile);
f_return 	configure_iochannels	(char *pConfigFile);
f_return	checkchannelconfig		(struct channel Ch);

#endif