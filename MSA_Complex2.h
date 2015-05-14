#ifndef __MSA_rules_ye_COMPLEX__
#define __MSA_rules_ye_COMPLEX__

typedef struct {
	float a,b;
} COMPLEX;

//float _fA,_fB;
COMPLEX tz;

/*********************** DEFS *************************************/

#define _CreateComplex(z,ta,tb)  { z.a=(ta); z.b=(tb); }  


#define _ModComplex		  		\
__asm	fld		st				\
__asm	fmulp	st(1),st		\
__asm	fxch					\
__asm	fld		st				\
__asm	fmulp	st(1),st		\
__asm	faddp	st(1),st		\
__asm	fsqrt


#define _Mod2Complex	  		\
__asm	fld		st				\
__asm	fmulp	st(1),st		\
__asm	fxch					\
__asm	fld		st				\
__asm	fmulp	st(1),st		\
__asm	faddp	st(1),st


#define _LoadComplex(z)			\
__asm	fld		dword ptr[z+4]	\
__asm	fld		dword ptr[z]


#define _SaveComplex(z)  		\
__asm	fstp	dword ptr [z]	\
__asm	fstp	dword ptr [z+4]


#define _AddComplex(z)  		\
__asm	fadd	dword ptr[z]	\
__asm	fxch					\
__asm	fadd	dword ptr[z+4]	\
__asm	fxch


#define _SubComplex(z)  		\
__asm	fsub	dword ptr[z]	\
__asm	fxch					\
__asm	fsub	dword ptr[z+4]	\
__asm	fxch


// (a1*a2-b1*b2 , a1*b2+a2*b1)
#define _MulComplex(z)  		\
__asm	fst		dword ptr[tz]	\
__asm	fxch					\
__asm	fst		dword ptr[tz+4]	\
__asm	fmul	dword ptr[z+4]	\
__asm	fxch					\
__asm	fmul	dword ptr[z]	\
__asm	fxch					\
__asm	fsubp	st(1),st		\
__asm	fld		dword ptr[tz]	\
__asm	fmul	dword ptr[z+4]	\
__asm	fld		dword ptr[tz+4]	\
__asm	fmul	dword ptr[z]	\
__asm	faddp	st(1),st		\
__asm	fxch

/*********************************************************************/
#endif // __MSA_rules_ye_COMPLEX__