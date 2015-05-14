#include "MSA_complex.h"

/*********************** MISC OPERATIONS **************************/

__declspec(naked) float __fastcall mod(MSA_COMPLEX &z)
{
	__asm {
		fld		dword ptr[ecx]
		fmul	dword ptr[ecx]
		fld		dword ptr[ecx+4]
		fmul	dword ptr[ecx+4]
		faddp	st(1),st
		fsqrt
		ret
	}
}

__declspec(naked) float __fastcall mod2(MSA_COMPLEX &z)
{
	__asm {
		fld		dword ptr[ecx]
		fmul	dword ptr[ecx]
		fld		dword ptr[ecx+4]
		fmul	dword ptr[ecx+4]
		faddp	st(1),st
		ret
	}
}


__declspec(naked) MSA_COMPLEX __stdcall Complex(float ta,float tb)
{
	__asm {
		mov		eax,dword ptr[esp+4]
		mov		edx,dword ptr[esp+8]
		ret		8
	}
}


__declspec(naked) MSA_COMPLEX __fastcall operator !(MSA_COMPLEX& z)
{
	__asm {
		mov		eax,dword ptr[ecx]
		fld		dword ptr[ecx+4]
		fchs
		fstp	dword ptr[esp-4]
		mov		edx,dword ptr[esp-4]
		ret
	}
	
}

/*********************** FLOAT OPERATIONS **************************/

__declspec(naked) void __fastcall operator +=(MSA_COMPLEX &z1,float f)			// (a1+f	,	b1)
{
	__asm {
		fld		dword ptr[ecx]
		fadd	dword ptr[esp+4]
		fstp	dword ptr[ecx]
		ret		4
	}
}

__declspec(naked) void __fastcall operator -=(MSA_COMPLEX &z1,float f)			// (a1-f	,	b1)
{
	__asm {
		fld		dword ptr[ecx]
		fsub	dword ptr[esp+4]
		fstp	dword ptr[ecx]
		ret		4
	}
}

__declspec(naked) void __fastcall operator *=(MSA_COMPLEX &z1,float f)			//	(a1*f	,	b1*f)
{
	__asm{
		fld		dword ptr[ecx]
		fmul	dword ptr[esp+4]
		fstp	dword ptr[ecx]
		fld		dword ptr[ecx+4]
		fmul	dword ptr[esp+4]
		fstp	dword ptr[ecx+4]
		ret		4
	}
}

__declspec(naked) void __fastcall operator /=(MSA_COMPLEX &z1,float f)			//	(a1/f	,	b1/f)
{
	__asm{
		fld1	
		fld		dword ptr[esp+4]
		fdivp	st(1),st
		fstp	dword ptr[esp+4]
		fld		dword ptr[ecx]
		fmul	dword ptr[esp+4]
		fstp	dword ptr[ecx]
		fld		dword ptr[ecx+4]
		fmul	dword ptr[esp+4]
		fstp	dword ptr[ecx+4]
		ret		4
	}
}


__declspec(naked) MSA_COMPLEX __fastcall operator + (MSA_COMPLEX &z1,float f)		// (a1+f	,	b1)
{
	__asm {
		fld		dword ptr[ecx]
		fadd	dword ptr[esp+4]
		fstp	dword ptr[esp-4]
		mov		eax,dword ptr[esp-4]
		mov		edx,dword ptr[ecx+4]
		ret		4
	}
}

__declspec(naked) MSA_COMPLEX __fastcall operator - (MSA_COMPLEX &z1,float f)		// (a1-f	,	b1)
{
	__asm {
		fld		dword ptr[ecx]
		fsub	dword ptr[esp+4]
		fstp	dword ptr[esp-4]
		mov		eax,dword ptr[esp-4]
		mov		edx,dword ptr[ecx+4]
		ret		4
	}
}

__declspec(naked) MSA_COMPLEX __fastcall operator * (MSA_COMPLEX &z1,float f)		//	(a1*f	,	b1*f)
{
	__asm{
		fld		dword ptr[ecx]
		fmul	dword ptr[esp+4]
		fstp	dword ptr[esp-8]
		fld		dword ptr[ecx+4]
		fmul	dword ptr[esp+4]
		fstp	dword ptr[esp-4]
		mov		eax,dword ptr[esp-8]
		mov		edx,dword ptr[esp-4]
		ret		4
	}
}

__declspec(naked) MSA_COMPLEX __fastcall operator / (MSA_COMPLEX &z1,float f)		//	(a1/f	,	b1/f)
{
	__asm{
		fld1	
		fld		dword ptr[esp+4]
		fdivp	st(1),st
		fstp	dword ptr[esp+4]
		fld		dword ptr[ecx]
		fmul	dword ptr[esp+4]
		fstp	dword ptr[esp-8]
		fld		dword ptr[ecx+4]
		fmul	dword ptr[esp+4]
		fstp	dword ptr[esp-4]
		mov		eax,dword ptr[esp-8]
		mov		edx,dword ptr[esp-4]
		ret		4
	}
}



/*********************** MSA_COMPLEX OPERATIONS **************************/

__declspec(naked) void __fastcall operator +=(MSA_COMPLEX &z1,MSA_COMPLEX &z2)		// (a1+a2	,	b1+b2)
{
	__asm {
		fld		dword ptr[ecx]
		fadd	dword ptr[edx]
		fstp	dword ptr[ecx]
		fld		dword ptr[ecx+4]
		fadd	dword ptr[edx+4]
		fstp	dword ptr[ecx+4]
		ret
	}
}

__declspec(naked) void __fastcall operator -=(MSA_COMPLEX &z1,MSA_COMPLEX &z2)		// (a1-a2	,	b1-b2)
{
	__asm {
		fld		dword ptr[ecx]
		fsub	dword ptr[edx]
		fstp	dword ptr[ecx]
		fld		dword ptr[ecx+4]
		fsub	dword ptr[edx+4]
		fstp	dword ptr[ecx+4]
		ret
	}
}

__declspec(naked) void __fastcall operator *=(MSA_COMPLEX &z1,MSA_COMPLEX &z2)		// (a1*a2-b1*b2 , a1*b2+a2*b1)
{
	__asm{
		fld		dword ptr[ecx]
		fmul	dword ptr[edx]
		fld		dword ptr[ecx+4]
		fmul	dword ptr[edx+4]
		fsubp	st(1),st
		fld		dword ptr[ecx]
		fmul	dword ptr[edx+4]
		fld		dword ptr[ecx+4]
		fmul	dword ptr[edx]
		faddp	st(1),st
		fstp	dword ptr[ecx+4]
		fstp	dword ptr[ecx]
		ret
	}
}


__declspec(naked) void __fastcall operator /= (MSA_COMPLEX &z1,MSA_COMPLEX &z2)	// (a1*a2+b1*b2)/tf, (a2*b1-a1*b2)/tf); tf=a2*a2+b2*b2;
{
	__asm{
		fld		dword ptr[edx]
		fmul	dword ptr[edx]
		fld		dword ptr[edx+4]
		fmul	dword ptr[edx+4]
		faddp	st(1),st
		fld1
		fdivrp	st(1),st
		fstp	dword ptr[esp-12]
		fld		dword ptr[ecx]
		fmul	dword ptr[edx]
		fld		dword ptr[ecx+4]
		fmul	dword ptr[edx+4]
		faddp	st(1),st
		fmul	dword ptr[esp-12]
		fld		dword ptr[ecx+4]
		fmul	dword ptr[edx]
		fld		dword ptr[ecx]
		fmul	dword ptr[edx+4]
		fsubp	st(1),st
		fmul	dword ptr[esp-12]
		fstp	dword ptr[ecx+4]
		fstp	dword ptr[ecx]
		ret
	}
}

__declspec(naked) MSA_COMPLEX __fastcall operator + (MSA_COMPLEX &z1,MSA_COMPLEX &z2)	// (a1+a2	,	b1+b2)
{
	__asm {
		fld		dword ptr[ecx]
		fadd	dword ptr[edx]
		fstp	dword ptr[esp-8]
		fld		dword ptr[ecx+4]
		fadd	dword ptr[edx+4]
		fstp	dword ptr[esp-4]
		mov		eax,dword ptr[esp-8]
		mov		edx,dword ptr[esp-4]
		ret
	}
}


__declspec(naked) MSA_COMPLEX __fastcall operator - (MSA_COMPLEX &z1,MSA_COMPLEX &z2)	// (a1-a2	,	b1-b2)
{
	__asm {
		fld		dword ptr[ecx]
		fsub	dword ptr[edx]
		fstp	dword ptr[esp-8]
		fld		dword ptr[ecx+4]
		fsub	dword ptr[edx+4]
		fstp	dword ptr[esp-4]
		mov		eax,dword ptr[esp-8]
		mov		edx,dword ptr[esp-4]
		ret
	}
}



__declspec(naked) MSA_COMPLEX __fastcall operator * (MSA_COMPLEX &z1,MSA_COMPLEX &z2)	// (a1*a2-b1*b2 , a1*b2+a2*b1)
{
	__asm{
		fld		dword ptr[ecx]
		fmul	dword ptr[edx]
		fld		dword ptr[ecx+4]
		fmul	dword ptr[edx+4]
		fsubp	st(1),st
		fld		dword ptr[ecx]
		fmul	dword ptr[edx+4]
		fld		dword ptr[ecx+4]
		fmul	dword ptr[edx]
		faddp	st(1),st
		fstp	dword ptr[esp-4]
		fstp	dword ptr[esp-8]
		mov		eax,dword ptr[esp-8]
		mov		edx,dword ptr[esp-4]
		ret
	}
}

__declspec(naked) MSA_COMPLEX __fastcall operator / (MSA_COMPLEX &z1,MSA_COMPLEX &z2)	// (a1*a2+b1*b2)/tf, (a2*b1-a1*b2)/tf); tf=a2*a2+b2*b2;
{
	__asm{
		fld		dword ptr[edx]
		fmul	dword ptr[edx]
		fld		dword ptr[edx+4]
		fmul	dword ptr[edx+4]
		faddp	st(1),st
		fld1
		fdivrp	st(1),st
		fstp	dword ptr[esp-12]
		fld		dword ptr[ecx]
		fmul	dword ptr[edx]
		fld		dword ptr[ecx+4]
		fmul	dword ptr[edx+4]
		faddp	st(1),st
		fmul	dword ptr[esp-12]
		fstp	dword ptr[esp-8]
		fld		dword ptr[ecx+4]
		fmul	dword ptr[edx]
		fld		dword ptr[ecx]
		fmul	dword ptr[edx+4]
		fsubp	st(1),st
		fmul	dword ptr[esp-12]
		fstp	dword ptr[esp-4]
		mov		eax,dword ptr[esp-8]
		mov		edx,dword ptr[esp-4]
		ret
	}
}

__declspec(naked) float __fastcall operator ^ (MSA_COMPLEX &z1,MSA_COMPLEX &z2)			// (a1*a2+b1*b2) dot product
{
	__asm{
		fld		dword ptr[ecx]
		fmul	dword ptr[edx]
		fld		dword ptr[ecx+4]
		fmul	dword ptr[edx+4]
		faddp	st(1),st
		ret
	}
}
