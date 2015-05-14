#include <math.h>

#include "MSA_Utils.h"
#include "MSA_Fractal.h"

MSA_FRACTAL::MSA_FRACTAL(float tfH,float tfLacunarity,int tiOctaves)
{
	_ClearMe;
	Init(tfH,tfLacunarity,tiOctaves);
}

MSA_FRACTAL::~MSA_FRACTAL()
{
	_del(pfFractPow);
}

void MSA_FRACTAL::Init(float tfH,float tfLacunarity,int tiOctaves)
{
    fH=tfH;
	fLacunarity=tfLacunarity;
	iOctaves=tiOctaves;
	_del(pfFractPow);
	float f=1;
	pfFractPow=new float[iOctaves];
	if(!pfFractPow) _FUCKER;
    for(int c=0; c<iOctaves; c++) {
        pfFractPow[c] = (float)(1/pow(f,(1-fH)));
        f*=fLacunarity;
    }
}

void MSA_FRACTAL::SetFn2(float (*tfn)(float,float))
{
	fn2=tfn;
}

void MSA_FRACTAL::SetFn3(float (*tfn)(float,float,float))
{
	fn3=tfn;
}


float fOne	=1.0f;
float fHalf	=0.5f;

__declspec(naked) float MSA_FRACTAL::fBm(float x,float y)
{
/*    float fRet=0;
	for(int c=0; c<iOctaves; c++) {
		fRet += fn2(x,y)*pfFractPow[c];
        x *= fLacunarity;
        y *= fLacunarity;
    }
    return (fRet+1)/2;*/
	//	float fRet;				ebp-4
	//	int c;					ebp-8
	__asm {
		push	ebp
		mov		ebp,esp
		sub		esp,12

		mov		dword ptr[ebp-8],0			// for counter
		mov		dword ptr[ebp-4],0			// for fRet
		mov		dword ptr[ebp-12],ecx		// for this

		push	dword ptr[ebp+12]
		push	dword ptr[ebp+8]

	LoopBegin:
		call	dword ptr[ecx+16]

		mov		ecx,dword ptr[ebp-12]		// address of pfFractpow
		mov		eax,dword ptr[ebp-8]		// value of counter
		mov		edx,dword ptr[ecx]

		fmul	dword ptr[edx+4*eax]		// noise*pfFractpow[c]		
		fadd	dword ptr[ebp-4]			// fret+fRet
		fstp	dword ptr[ebp-4]			// save fRet

		fld		dword ptr[esp]				// load x
		fmul	dword ptr[ecx+8]			// x*fLacunarity
		fstp	dword ptr[esp]				// save x

		fld		dword ptr[esp+4]			// load y
		fmul	dword ptr[ecx+8]			// y*fLacunarity
		fstp	dword ptr[esp+4]			// save y

		inc		eax
		inc		dword ptr[ebp-8]
		cmp		eax,dword ptr[ecx+12]
		jl		LoopBegin

		fld		dword ptr[ebp-4]
		fadd	dword ptr[fOne]
		fmul	dword ptr[fHalf]
		mov		esp,ebp
		pop		ebp
		ret		8
	}
}

__declspec(naked) float MSA_FRACTAL::fBm(float x,float y, float z)
{
/*    float fRet=0;
	for(int c=0; c<iOctaves; c++) {
		fRet += fn3(x,y,z)*pfFractPow[c];
        x *= fLacunarity;
        y *= fLacunarity;
        z *= fLacunarity;
    }
    return (fRet+1)/2;*/
//	float fRet;				ebp-4
//	int c;					ebp-8
	__asm {
		push	ebp
		mov		ebp,esp
		sub		esp,12

		mov		dword ptr[ebp-8],0			// for counter
		mov		dword ptr[ebp-4],0			// for fRet
		mov		dword ptr[ebp-12],ecx		// for this

		push	dword ptr[ebp+16]
		push	dword ptr[ebp+12]
		push	dword ptr[ebp+8]

	LoopBegin:
		call	dword ptr[ecx+20]

		mov		ecx,dword ptr[ebp-12]		// address of pfFractpow
		mov		eax,dword ptr[ebp-8]		// value of counter
		mov		edx,dword ptr[ecx]

		fmul	dword ptr[edx+4*eax]		// noise*pfFractpow[c]		
		fadd	dword ptr[ebp-4]			// fret+fRet
		fstp	dword ptr[ebp-4]			// save fRet

		fld		dword ptr[esp]				// load x
		fmul	dword ptr[ecx+8]			// x*fLacunarity
		fstp	dword ptr[esp]				// save x

		fld		dword ptr[esp+4]			// load y
		fmul	dword ptr[ecx+8]			// y*fLacunarity
		fstp	dword ptr[esp+4]			// save y

		fld		dword ptr[esp+8]			// load z
		fmul	dword ptr[ecx+8]			// z*fLacunarity
		fstp	dword ptr[esp+8]			// save z


/*		fmul	dword ptr[edx+4*eax]		// noise*pfFractpow[c]		
		fld		dword ptr[esp]				// x
		fmul	dword ptr[ecx+8]			// x*fLacunarity
		fxch
		fadd	dword ptr[ebp-4]			// fret+fRet
		fld		dword ptr[esp+4]			// y
		fmul	dword ptr[ecx+8]			// y*fLacunarity
		fxch
		fstp	dword ptr[ebp-4]			// save fRet
		fxch
		fstp	dword ptr[esp]				// x
		fld		dword ptr[esp+8]			// z
		fmul	dword ptr[ecx+8]			// z*fLacunarity
		fxch
		fstp	dword ptr[esp+4]			// y
		fstp	dword ptr[esp+8]			// z
*/


		inc		eax
		inc		dword ptr[ebp-8]
		cmp		eax,dword ptr[ecx+12]
		jl		LoopBegin

		fld		dword ptr[ebp-4]
		fadd	dword ptr[fOne]
		fmul	dword ptr[fHalf]
		mov		esp,ebp
		pop		ebp
		ret		12
	}
}

