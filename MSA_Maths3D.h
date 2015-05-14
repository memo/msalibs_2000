#ifndef __MSA_rules_ye_Maths__
#define __MSA_rules_ye_Maths__

#include <math.h>


/***************************** Misc *************************************/

#define MSA_pF(vv)			( (float*)(&vv) )				// cast as pointer to float

/*
#define _Step(x,a)			((x)>=(a)?1:0)					// return 1 or 0 depending on sign
#define _Sign(x)			( x ? (x < 0 ? -1 : 1) : 0 )	// return -1 or 1 depending on sign
//#define _Sign2(x, t)		( x<-t ? -1 : (x>t ? 1 : 0) )		 


#define _Min(a,b)			__min(a,b)									// ((a)<(b)?(a):(b))
#define _Max(a,b)			__max(a,b)									// ((a)<(b)?(b):(a))
#define _Abs(x)				(float)fabs(x)								// ((x)<0?(-(x)):(x))

#define _Floor(x)			(float)floor(x)								// ((int)(x) - ((x) < 0 && (x) != (int)(x)))
#define _Ceil(x)			(float)ceil(x)								// ((int)(x) + ((x) > 0 && (x) != (int)(x)))

#define _Lerp(x,a,b)		((a) + (x)*((b)-(a)))						// linear interpolate
#define _Smooth01(x)		((x)*(x)*(3 - 2*(x)))						// smooth a 0...1 number at the ends 0 and 1

#define _In(x,a,b)			((x>=a) && (x<=b))							// return true if x is in the range a...b
#define _Clamp(x,a,b)		if(x<a) x=a; else if(x>b) x=b;				// clamp a value x to the range a...b


#define _BoxStep(x,a,b)		_Clamp( ((x)-(a)) / (float)((b)-(a)) ,0, 1)	// return 0...1 depending on where x is relative to a...b
#define _SmoothStep(x,a,b)	_Smooth01(_BoxStep(a,b,x))

#define _PULSE(x,a,b)		(step(a,x) - step(b,x))

#define _Bias(b,x)			powf((x),logf(b)/_LOG05)
#define _Gain(g,x)			((x)<0.5)?(bias(1-(g),2*(x))/2):(1-bias(1-(g),(2)-(2)*x)/2)
#define _Gamma(g,x)			powf((x),1/(g));

#define _Rand01				((rand()&0xFF)/(float)0xFF)
*/


#define PI					3.14159265359f
#define TWO_PI				(2.0f * PI)
#define HALF_PI				(0.5f * PI)
#define INV_PI				(1.0f / PI)

#define	DEG2RAD				(PI / 180.0f)
#define RAD2DEG				(180.0f / PI)


//#define LOG05				-0.693147180559945f

#define _Sin(Rad)			(float)sin(Rad)
#define _Cos(Rad)			(float)cos(Rad)


/*
static float SinArray[65536];
static float CosArray[65536];

inline float _Sin(float fRad)	{ _Clamp(fRad, 0, TWOPI); return SinArray[(WORD)(fRad * 65536.0f/TWOPI)]; };
inline float _Cos(float fRad)	{ _Clamp(fRad, 0, TWOPI); return CosArray[(WORD)(fRad * 65536.0f/TWOPI)]; };
*/


int __inline MSA_InitMath()
{
/*	for(int c=0; c<65536; c++) {
		SinArray[c] = (float)sin(c * TWOPI / 65536.0f);
		CosArray[c] = (float)cos(c * TWOPI / 65536.0f);
	}
*/
	return TRUE;
}




/************************************** Vectors *************************************************/

typedef struct { float x, y; }			MSA_FVECT2;				// float Vector2
typedef struct { int x, y; }			MSA_IVECT2;				// int Vector2

typedef struct { float x, y, z;	}		MSA_FVECT3;				// float Vector3
typedef struct { int x, y, z; }			MSA_IVECT3;				// int Vector3

typedef struct { float x, y, z, w; }	MSA_FVECT4;				// float Vector4
typedef struct { int x, y, z, w; }		MSA_IVECT4;				// int Vector4


#define MSA_fV2(vv)					(*(MSA_FVECT2*)(&vv))	// cast vv as MSA_FVECT2
#define MSA_fV3(vv)					(*(MSA_FVECT3*)(&vv))	// cast vv as MSA_FVECT3
#define MSA_fV4(vv)					(*(MSA_FVECT4*)(&vv))	// cast vv as MSA_FVECT4

#define MSA_iV2(vv)					(*(MSA_IVECT2*)(&vv))	// cast vv as MSA_IVECT2
#define MSA_iV3(vv)					(*(MSA_IVECT3*)(&vv))	// cast vv as MSA_IVECT3
#define MSA_iV4(vv)					(*(MSA_IVECT4*)(&vv))	// cast vv as MSA_IVECT4


#define MSA_SetV2(vv,X,Y)			{ vv.x = X;	  vv.y = Y; }
#define MSA_SetV3(vv,X,Y,Z)			{ vv.x = X;   vv.y = Y;   vv.z = Z; }
#define MSA_SetV4(vv,X,Y,Z,W)		{ vv.x = X;   vv.y = Y;   vv.z = Z;   vv.w = W; }

#define	MSA_IncV2(v1,v2)			{ v1.x += v2.x;   v1.y += v2.y; }
#define	MSA_IncV3(v1,v2)			{ v1.x += v2.x;   v1.y += v2.y;   v1.z += v2.z; }
#define	MSA_IncV4(v1,v2)			{ v1.x += v2.x;   v1.y += v2.y;   v1.z += v2.z;   v1.w += v2.w; }

#define	MSA_IncV2f(v,X,Y)			{ v.x += X;   v.y += Y; }
#define	MSA_IncV3f(v,X,Y,Z)			{ v.x += X;   v.y += Y;   v.z += Z; }
#define	MSA_IncV4f(v,X,Y,Z,W)		{ v.x += X;   v.y += Y;   v.z += Z;   v.w += W; }

#define	MSA_AddV2(v1,v2,vr)			{ vr.x = v1.x + v2.x;   vr.y = v1.y + v2.y; }
#define	MSA_AddV3(v1,v2,vr)			{ vr.x = v1.x + v2.x;   vr.y = v1.y + v2.y;   vr.z = v1.z + v2.z; }
#define	MSA_AddV4(v1,v2,vr)			{ vr.x = v1.x + v2.x;   vr.y = v1.y + v2.y;   vr.z = v1.z + v2.z;   vr.w = v1.w + v2.w; }

#define	MSA_AddV2f(v1,X,Y,vr)		{ vr.x = v1.x + X;   vr.y = v1.y + Y; }
#define	MSA_AddV3f(v1,X,Y,Z,vr)		{ vr.x = v1.x + X;   vr.y = v1.y + Y;   vr.z = v1.z + Z; }
#define	MSA_AddV4f(v1,X,Y,Z,W,vr)	{ vr.x = v1.x + X;   vr.y = v1.y + Y;   vr.z = v1.z + Z;   vr.w = v1.w + W; }

#define	MSA_DecV2(v1,v2)			{ v1.x -= v2.x;   v1.y -= v2.y; }
#define	MSA_DecV3(v1,v2)			{ v1.x -= v2.x;   v1.y -= v2.y;   v1.z -= v2.z; }
#define	MSA_DecV4(v1,v2)			{ v1.x -= v2.x;   v1.y -= v2.y;   v1.z -= v2.z;   v1.w -= v2.w; }

#define	MSA_SubV2(v1,v2,vr)			{ vr.x = v1.x - v2.x;   vr.y = v1.y - v2.y; }
#define	MSA_SubV3(v1,v2,vr)			{ vr.x = v1.x - v2.x;   vr.y = v1.y - v2.y;   vr.z = v1.z - v2.z; }
#define	MSA_SubV4(v1,v2,vr)			{ vr.x = v1.x - v2.x;   vr.y = v1.y - v2.y;   vr.z = v1.z - v2.z;   vr.w = v1.w - v2.w; }

#define MSA_MulV2f(vv,f)			{ vv.x *= f;   vv.y *= f; }
#define MSA_MulV3f(vv,f)			{ vv.x *= f;   vv.y *= f;   vv.z *= f; }
#define MSA_MulV4f(vv,f)			{ vv.x *= f;   vv.y *= f;   vv.z *= f;   vv.w *= f; }

#define	MSA_LenV2(vv)				((float)sqrt(vv.x*vv.x + vv.y*vv.y))
#define	MSA_LenV3(vv)				((float)sqrt(vv.x*vv.x + vv.y*vv.y + vv.z*vv.z))
#define	MSA_LenV4(vv)				((float)sqrt(vv.x*vv.x + vv.y*vv.y + vv.z*vv.z + vv.w*vv.w))

#define	MSA_NormV2(vv)				{ float f = (1.0f/MSA_LenV2(vv)); MSA_MulV2f(vv, f); }
#define	MSA_NormV3(vv)				{ float f = (1.0f/MSA_LenV3(vv)); MSA_MulV3f(vv, f); }
#define	MSA_NormV4(vv)				{ float f = (1.0f/MSA_LenV4(vv)); MSA_MulV4f(vv, f); }

#define MSA_DotV2(v1,v2)			( v1.x*v2.x + v1.y*v2.y )
#define MSA_DotV3(v1,v2)			( v1.x*v2.x + v1.y*v2.y + v1.z*v2.z )
#define MSA_DotV4(v1,v2)			( v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w)


// CAN BE SAME VECT3
#define MSA_CrossV3(v1,v2,vr)		{ MSA_FVECT3 v;	v.x=v1.y*v2.z-v1.z*v2.y; v.z=v1.x*v2.y-v1.y*v2.x; v.y=v1.z*v2.x-v1.x*v2.z; MSA_SetV3(vr, v.x, v.y, v.z); }
/*float __inline __fastcall MSA_CrossV3(MSA_FVECT3 &v1, MSA_FVECT3 &v2, MSA_FVECT3 &vr)
{
	MSA_FVECT3 v;
	v.x = v1.y * v2.z - v1.z * v2.y;
	v.z = v1.x * v2.y - v1.y * v2.x;
	v.y = v1.z * v2.x - v1.x * v2.z;
	MSA_SetV3(vr, v.x, v.y, v.z);
}
*/

// Calculate normal of triangle given by points v1,v2,v3 and store in vR
void __inline __fastcall MSA_CalcNorm(MSA_FVECT3 &v1, MSA_FVECT3 &v2, MSA_FVECT3 &v3, MSA_FVECT3 &vR)				
{
	MSA_FVECT3 Edge1 = v2;
	MSA_FVECT3 Edge2 = v3;

	MSA_DecV3(Edge1, v1);				// Edge1 = v2 - v1;
	MSA_DecV3(Edge2, v1);				// Edge2 = v3 - v1;
	
	MSA_CrossV3(Edge1, Edge2, vR);		// vR = Edge1 x Edge2

	MSA_NormV3(vR);
}



/************************************* Matrices ***********************************************/

typedef struct {
	MSA_FVECT4	x;
	MSA_FVECT4	y;
	MSA_FVECT4	z;
	MSA_FVECT4	t;

	BOOL bChanged;

} MSA_MATRIX44;

const	MSA_MATRIX44						MSA_m44Identity = { {1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1}, TRUE };


#define MSA_InitM44(m44)					{ m44 = MSA_m44Identity; }
#define MSA_InitRotM44(m44)					{ MSA_SetV3(m44.x, 1, 0, 0);  MSA_SetV3(m44.y, 0, 1, 0);  MSA_SetV3(m44.z, 0, 0, 1); m44.bChanged = TRUE; }
#define MSA_InitPosM44(m44)					{ MSA_SetPosM44(m44, 0, 0, 0); m44.bChanged = TRUE; }

#define MSA_SetPosM44(m44,X,Y,Z)			{ MSA_SetV3(m44.t, X, Y, Z);  m44.bChanged = TRUE; }

#define MSA_MoveM44f(m44,X,Y,Z)				{ MSA_IncV3f(m44.t, X, Y, Z); m44.bChanged = TRUE; }	
#define MSA_MoveM44(m44,v3, fDist)			{ MSA_MoveM44f(m44, v3.x*fDist, v3.y*fDist, v3.z*fDist) }



// CANNOT BE SAME MATRIX
void __inline __fastcall MSA_InvM44( MSA_MATRIX44 &m44S, MSA_MATRIX44 &m44R )
{
	// Set the last column
	m44R.x.w = 0.0f;
	m44R.y.w = 0.0f;
	m44R.z.w = 0.0f;
	m44R.t.w = 1.0f;

	// Transform the position
	m44R.t.x = - MSA_DotV3( m44S.x, m44S.t );
	m44R.t.y = - MSA_DotV3( m44S.y, m44S.t );
	m44R.t.z = - MSA_DotV3( m44S.z, m44S.t );

	// Transform the rotation
	m44R.x.x = m44S.x.x;
	m44R.y.y = m44S.y.y;
	m44R.z.z = m44S.z.z;

	m44R.x.y = m44S.y.x;
	m44R.y.x = m44S.x.y;

	m44R.x.z = m44S.z.x;
	m44R.z.x = m44S.x.z;

	m44R.y.z = m44S.z.y;
	m44R.z.y = m44S.y.z;

	m44R.bChanged = TRUE;
}


/*					 X	 Y	 Z	 W

Xx	Yx	Zx	Wx		 1	 0	 0	 0	 x
Xy	Yy	Zy	Wy		 0	 C	-S	 0	 y
Xz	Yz	Zz	Wz		 0	 S	 C	 0	 z
Xt	Yt	Zt	Wt		 0	 0	 0	 1	 t
*/
void __inline __fastcall MSA_RotM44_X( MSA_MATRIX44 &m44, float fRad )
{
	float C = _Cos(fRad);
	float S = _Sin(fRad);

	float Yx = m44.y.x;
	float Yy = m44.y.y;
	float Yz = m44.y.z;

	m44.y.x = Yx * C + m44.z.x * S;
	m44.z.x = Yx *-S + m44.z.x * C;

	m44.y.y = Yy * C + m44.z.y * S;
	m44.z.y = Yy *-S + m44.z.y * C;

	m44.y.z = Yz * C + m44.z.z * S;
	m44.z.z = Yz *-S + m44.z.z * C;

	m44.bChanged = TRUE; 
}

/*					 X	 Y	 Z	 W

Xx	Yx	Zx	Wx		 C	 0	-S	 0	 x
Xy	Yy	Zy	Wy		 0	 1 	 0	 0	 y
Xz	Yz	Zz	Wz		 S	 0	 C	 0	 z
Xt	Yt	Zt	Wt		 0	 0	 0	 1	 t
*/
void __inline __fastcall MSA_RotM44_Y( MSA_MATRIX44 &m44, float fRad )
{
	float C = _Cos(fRad);
	float S = _Sin(fRad);

	float Yx = m44.y.x;
	float Yy = m44.y.y;
	float Yz = m44.y.z;

	m44.y.x = Yx * C + m44.z.x * S;
	m44.z.x = Yx *-S + m44.z.x * C;

	m44.y.y = Yy * C + m44.z.y * S;
	m44.z.y = Yy *-S + m44.z.y * C;

	m44.y.z = Yz * C + m44.z.z * S;
	m44.z.z = Yz *-S + m44.z.z * C;

	m44.bChanged = TRUE; 
}

/*					 X	 Y	 Z	 W

Xx	Yx	Zx	Wx		 C	-S	 0	 0	 x
Xy	Yy	Zy	Wy		 S	 C	 0	 0	 y
Xz	Yz	Zz	Wz		 0	 0	 1	 0	 z
Xt	Yt	Zt	Wt		 0	 0	 0	 1	 t
*/
void __inline __fastcall MSA_RotM44_Z( MSA_MATRIX44 &m44, float fRad )
{
	float C = _Cos(fRad);
	float S = _Sin(fRad);

	float Xx = m44.x.x;
	float Xy = m44.x.y;
	float Xz = m44.z.z;

	m44.x.x = Xx * C + m44.y.x * S;
	m44.y.x = Xx *-S + m44.y.x * C;

	m44.x.y = Xy * C + m44.y.y * S;
	m44.y.y = Xy *-S + m44.y.y * C;

	m44.x.z = Xz * C + m44.y.z * S;
	m44.y.z = Xz *-S + m44.y.z * C;

	m44.bChanged = TRUE; 
}





/************************************* Quaternions ***********************************************/

typedef struct { 

	float x, y, z, w; 
	BOOL bChanged;

}	MSA_QUAT;



const	MSA_QUAT							MSA_QuatIdentity = {0,0,0,1,TRUE};

#define MSA_SetQUAT(q,X,Y,Z,W)				{ q.x = X;  q.y = Y;  q.z = Z;  q.w = W;  q.bChanged = TRUE; }
//#define MSA_InitQUAT(q)						{ MSA_SetQUAT(q, 0, 0, 0, 1); }
#define MSA_InitQUAT(q)						{ q = MSA_QuatIdentity; }

#define MSA_InvQUAT(qS,qD)					{ MSA_SetQUAT(qD, -qS.x, -qS.y, -qS.z, qS.w); }

#define MSA_RotQUAT_f(qA,X,Y,Z,fRad)		{ MSA_QUAT qt;	MSA_NewQUAT(qt, X, Y, Z, fRad);		 MSA_MulQUAT(qA, qt, qA); }
#define MSA_RotQUAT_v3(qA,v3,fRad)			{ MSA_QUAT qt;	MSA_NewQUAT(qt, MSA_fV3(v3), fRad);  MSA_MulQUAT(qA, qt, qA); }



// Create Quat from axis (x,y,z) and angle fRad and store in qR
void __inline __fastcall MSA_NewQUAT(MSA_QUAT &qR, float x, float y, float z, float fRad)
{
	float s2 = _Sin(0.5f * fRad);

	qR.x    = x * s2;
	qR.y    = y * s2;
	qR.z    = z * s2;
	qR.w    = _Cos(0.5f * fRad);

//	MSA_NormV4(qR);
	qR.bChanged = TRUE; 
}


// Create Quat from axis (v3Axis) and angle fRad and store in qR
void __inline __fastcall MSA_NewQUAT(MSA_QUAT &qR, MSA_FVECT3 &v3Axis, float fRad)
{
	float s2 = _Sin(0.5f * fRad);

	qR.x    = v3Axis.x * s2;
	qR.y    = v3Axis.y * s2;
	qR.z    = v3Axis.z * s2;
	qR.w    = _Cos(0.5f * fRad);

//	MSA_NormV4(qR);
	qR.bChanged = TRUE; 
}   


// CAN BE SAME TARGET
//w1.v2 + w2.v1 + v1 x v2,   w1.w2 - v1.v2
void __inline __fastcall MSA_MulQUAT(MSA_QUAT &qA, MSA_QUAT &qB, MSA_QUAT &qR)
{
	MSA_FVECT3 va = MSA_fV3(qA);	MSA_MulV3f(va, qB.w);
	MSA_FVECT3 vb = MSA_fV3(qB);	MSA_MulV3f(vb, qA.w);

	qR.w = qA.w * qB.w - MSA_DotV3(qA, qB);

	MSA_CrossV3(qA, qB, qR);
	
	MSA_IncV3(qR, va);
	MSA_IncV3(qR, vb);

//    MSA_NormV4(qR);
	qR.bChanged = TRUE; 
}




/*
		|       2     2										|
        | 1 - 2Y  - 2Z		2XY - 2ZW		2XZ + 2YW		|
        |													|
        |                         2     2					|
    M = | 2XY + 2ZW			1 - 2X  - 2Z	2YZ - 2XW		|
        |													|
        |										  2		2	|
        | 2XZ - 2YW			2YZ + 2XW		1 - 2X  - 2Y	|
        |                                              
*/
void __inline _fastcall MSA_Quat2M44(MSA_QUAT &q, MSA_MATRIX44 &m44)
{
	float xx	= q.x * q.x;
	float xy	= q.x * q.y;
	float xz	= q.x * q.z;
	float xw	= q.x * q.w;

	float yy	= q.y * q.y;
	float yz	= q.y * q.z;
	float yw	= q.y * q.w;

	float zz	= q.z * q.z;
	float zw	= q.z * q.w;

	m44.x.x		= 1 - 2 * ( yy + zz );
	m44.x.y		=     2 * ( xy - zw );
	m44.x.z		=     2 * ( xz + yw );

	m44.y.x		=     2 * ( xy + zw );
	m44.y.y		= 1 - 2 * ( xx + zz );
	m44.y.z		=     2 * ( yz - xw );

	m44.z.x		=     2 * ( xz - yw );
	m44.z.y		=     2 * ( yz + xw );
	m44.z.z		= 1 - 2 * ( xx + yy );
/*
	mat[3] = mat[7] = mat[11 = mat[12] = mat[13] = mat[14] = 0;
	mat[15]		= 1;
*/
	m44.bChanged = TRUE;
}





/*
inline __declspec(naked) int __fastcall _Floor(float &f)			// int i=(int)f; if(i>f) i--;
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
		ret	
	}
}

inline __declspec(naked) int __fastcall _Ceil(float &f)				// int i=(int)f; if(i>f) i--;
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
		ret
	}
}


/********************************************************************************/



#endif	// __MSA_rules_ye_Maths__
