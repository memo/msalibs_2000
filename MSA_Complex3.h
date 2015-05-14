#ifndef __MSA_rules_ye_COMPLEX__
#define __MSA_rules_ye_COMPLEX__

float _ta,_tb;

/*********************** DEFS *************************************/

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


#define _DupComplex		 		\
__asm	fld		st(1)			\
__asm	fld		st(1)			


#define _SqrComplex				\
__asm	fst		dword ptr[_ta]	\
__asm	fxch					\
__asm	fst		dword ptr[_tb]	\
__asm	fmul	dword ptr[_tb]	\
__asm	fxch					\
__asm	fmul	dword ptr[_ta]	\
__asm	fxch					\
__asm	fsubp	st(1),st		\
__asm	fld		dword ptr[_ta]	\
__asm	fmul	dword ptr[_tb]	\
__asm	fld		st				\
__asm	faddp	st(1),st		\
__asm	fxch


#define _LoadComplex(a,b)		\
__asm	fld		dword ptr[b]	\
__asm	fld		dword ptr[a]


#define _SaveComplex(a,b)  		\
__asm	fstp	dword ptr [a]	\
__asm	fstp	dword ptr [b]


#define _AddComplex(a,b)  		\
__asm	fadd	dword ptr[a]	\
__asm	fxch					\
__asm	fadd	dword ptr[b]	\
__asm	fxch


#define _SubComplex(a,b)  		\
__asm	fsub	dword ptr[a]	\
__asm	fxch					\
__asm	fsub	dword ptr[b]	\
__asm	fxch


// (a1*a2-b1*b2 , a1*b2+a2*b1)
#define _MulComplex(a,b)  		\
__asm	fst		dword ptr[_ta]	\
__asm	fxch					\
__asm	fst		dword ptr[_tb]	\
__asm	fmul	dword ptr[b]	\
__asm	fxch					\
__asm	fmul	dword ptr[a]	\
__asm	fxch					\
__asm	fsubp	st(1),st		\
__asm	fld		dword ptr[_ta]	\
__asm	fmul	dword ptr[b]	\
__asm	fld		dword ptr[_tb]	\
__asm	fmul	dword ptr[a]	\
__asm	faddp	st(1),st		\
__asm	fxch

/*********************************************************************/
#endif // __MSA_rules_ye_COMPLEX__