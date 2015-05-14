#ifndef __MSA_rules_ye_FRACTAL__
#define __MSA_rules_ye_FRACTAL__

class MSA_FRACTAL {
	public:	
		float *pfFractPow;					// 0

		float fH;							// +4
		float fLacunarity;					// +8
		int iOctaves;						// +12
	public:
		float (*fn2)(float,float);			// +16
		float (*fn3)(float,float,float);	// +20

		MSA_FRACTAL(float tfH=0,float tfLacunarity=2,int tiOctaves=1);
		~MSA_FRACTAL();
		
		void Init(float tfH,float tfLacunarity,int tiOctaves);
		void SetFn2(float (*tfn)(float,float));
		void SetFn3(float (*tfn)(float,float,float));

		float fBm(float x,float y);
		float fBm(float x,float y, float z);
};

#endif	// __MSA_rules_ye_FRACTAL__