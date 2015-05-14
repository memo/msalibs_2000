#include "..\_MSA_Complex\MSA_complex.h"

#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#pragma comment (lib, "winmm.lib")

//#include "MSA_math.h"
/*
inline __declspec(naked) int __fastcall Floor(float &f)				// int i=(int)f; if(i>f) i--;
{
	__asm {
		fld			dword ptr[ecx]
		frndint
		fcom		dword ptr[ecx]
		fistp		dword ptr[esp-4]
		fnstsw      ax
		test        ah,41h
		jne         ItsOk
		dec			dword ptr[esp-4]
	ItsOk:
		mov			eax,dword ptr[esp-4]
		ret			4
	}
}


inline int __fastcall Ceil(float &f)				// int i=(int)f; if(i<f) i++;
{
	__asm {
		fld			dword ptr[ecx]
		frndint
		fcom		dword ptr[ecx]
		fistp		dword ptr[esp-4]
		fnstsw      ax
		test        ah,1
		je         ItsOk
		inc			dword ptr[esp-4]
	ItsOk:
		mov			eax,dword ptr[esp-4]
		ret			4
	}
}
*/
/*inline int __fastcall Ceil(float f)	
{
	int i=(int)f; 
	if(i<f) i++;
	return i;
}
*/
void main()
{
	DWORD NowTime;

	MSA_COMPLEX z1={9.7f,8.3f},z2={3.6f,21.8f},z3={6.4f,13.4f},z4={14.0f,11.0f};

	float f = z1 ^ z2;


/*	float a1=9.7f	,b1=8.3f;
	float a2=3.6f	,b2=21.8f;
	float a3=6.4f	,b3=13.4f;
	float a4=14.0f	,b4=11.0f;
*/
/*
	NowTime=timeGetTime();
	for(int i=0; i<300000; i++) {
		z4=z1+z3*z2+3+(z2*2.5f+z3/2.7f)/z2-7.5f;
	}
	NowTime=timeGetTime()-NowTime;

	z4=z1+z2+z3;
	z4=z1/z2;
	z4=z1*z2;
	z4=!z1;
	z4=z1+z3*z2+3+(z2*2.5f+z3/2.7f)/z2-7.5f;

	z4=z1;
	z1*=2;

	z4=z1;
	z1*=z2;

	z4=z1;
	z1+=2;

	z4=z1;
	z1+=z2;

	z4=z1;
	z1-=2;

	z4=z1;
	z1-=z2;

	z4=z1;
	z1/=2;

	z4=z1;
	z1/=z2;
	float f=mod(z1);

*/
//	_CreateComplex(z1,5,8)
/*	_LoadComplex(a1,b1)
	_SqrComplex
	_DupComplex
	_AddComplex(a2,b2)
	_MulComplex(a4,b4)
	_SubComplex(a3,b3)
	_ModComplex
*/
	int i=0;

}