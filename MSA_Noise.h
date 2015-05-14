#ifndef __MSA_rules_ye_NOISE__
#define __MSA_rules_ye_NOISE__

#define	_NOISE_GRAD		1
#define	_NOISE_VAL		2
#define	_NOISE_ALL		255

void InitNoise(int iWhich=_NOISE_ALL, int iSeed=3131);

float gNoise(float x, float y);
float gNoise(float x, float y, float z);
float MSANoise(float x, float y, float z);


#endif // __MSA_rules_ye_NOISE__
