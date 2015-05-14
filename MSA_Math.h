#ifndef __MSA_rules_ye_MATH__
#define __MSA_rules_ye_MATH__

#include <math.h>

#define _Min(a,b)			((a)<(b)?(a):(b))
#define _Max(a,b)			((a)<(b)?(b):(a))
#define _Abs(x)				((x)<0?(-(x)):(x))

#define _Floor(x)			((int)(x) - ((x) < 0 && (x) != (int)(x)))
//#define _Ceil(x)			((int)(x) + ((x) > 0 && (x) != (int)(x)))

#define _Lerp(a,b,x)		((a) + (x)*((b)-(a)))
#define _Smooth01(x)		((x)*(x)*(3 - 2*(x)))

#define _In(x,a,b)			(((x)>=(a)) && ((x)<=(b)))
#define _Clamp(a,b,x)		((x)<=(a)?(a):((x)>=(b)?(b):(x)))

#define _Step(a,x)			((x)>=(a)?1:0)
#define _BoxStep(a,b,x)		_Clamp(0,1,((x)-(a))/(float)((b)-(a)))
#define _SmoothStep(a,b,x)	_Smooth01(_BoxStep(a,b,x))

#define _PULSE(a,b,x)		(step((a),(x)) - step((b),(x)))

#define _PI					3.1415926593f
#define _LOG05				-0.693147180559945f
#define _Bias(b,x)			powf((x),logf(b)/_LOG05)
#define _Gain(g,x)			((x)<0.5)?(bias(1-(g),2*(x))/2):(1-bias(1-(g),(2)-(2)*x)/2)
#define _Gamma(g,x)			powf((x),1/(g));

#define _Rand01				((rand()&0xFF)/(float)0xFF)

/*
int __fastcall _Floor(float &f);			// int i=(int)f; if(i>f) i--;
int __fastcall _Ceil(float &f);				// int i=(int)f; if(i<f) i++;
*/

#endif /* __MSA_rules_ye_MATH__*/
