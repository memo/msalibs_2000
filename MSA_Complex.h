#ifndef __MSA_rules_ye_COMPLEX__
#define __MSA_rules_ye_COMPLEX__

typedef struct {
	float x,y;
} MSA_COMPLEX;

/*********************** MISC OPERATIONS **************************/

MSA_COMPLEX __stdcall Complex(float ta,float tb);						// create complex
MSA_COMPLEX __fastcall operator !(MSA_COMPLEX& z);						// conjugate
float __fastcall mod(MSA_COMPLEX& z);									// length of complex
float __fastcall mod2(MSA_COMPLEX& z);									// length^2 of complex


/*********************** FLOAT OPERATIONS **************************/

void	__fastcall operator +=(MSA_COMPLEX &z1,float f);
void	__fastcall operator -=(MSA_COMPLEX &z1,float f);
void	__fastcall operator *=(MSA_COMPLEX &z1,float f);
void	__fastcall operator /=(MSA_COMPLEX &z1,float f);

MSA_COMPLEX __fastcall operator + (MSA_COMPLEX &z1,float f);			//	(a1+f	,	b1)
MSA_COMPLEX __fastcall operator - (MSA_COMPLEX &z1,float f);			//	(a1-f	,	b1)
MSA_COMPLEX __fastcall operator * (MSA_COMPLEX &z1,float f);			//	(a1*f	,	b1*f)
MSA_COMPLEX __fastcall operator / (MSA_COMPLEX &z1,float f);			//	(a1/f	,	b1/f)


/*********************** MSA_COMPLEX OPERATIONS **************************/

void	__fastcall operator +=(MSA_COMPLEX &z1,MSA_COMPLEX &z2);
void	__fastcall operator -=(MSA_COMPLEX &z1,MSA_COMPLEX &z2);
void	__fastcall operator *=(MSA_COMPLEX &z1,MSA_COMPLEX &z2);
void	__fastcall operator /=(MSA_COMPLEX &z1,MSA_COMPLEX &z2);

MSA_COMPLEX __fastcall operator + (MSA_COMPLEX &z1,MSA_COMPLEX &z2);	//	(a1+a2	,	b1+b2)
MSA_COMPLEX __fastcall operator - (MSA_COMPLEX &z1,MSA_COMPLEX &z2);	//	(a1-a2	,	b1-b2)
MSA_COMPLEX __fastcall operator * (MSA_COMPLEX &z1,MSA_COMPLEX &z2);	//	(a1*a2-b1*b2 , a1*b2+a2*b1)
MSA_COMPLEX __fastcall operator / (MSA_COMPLEX &z1,MSA_COMPLEX &z2);	//	(a1*a2+b1*b2)/tf, (a2*b1-a1*b2)/tf); tf=a2*a2+b2*b2;

float	__fastcall operator ^ (MSA_COMPLEX &z1,MSA_COMPLEX &z2);		//	(a1*a2+b1*b2)   dot product

/*********************************************************************/
#endif // __MSA_rules_ye_COMPLEX__