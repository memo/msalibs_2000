#include <math.h>
#include <stdlib.h>

#include "MSA_Utils.h"
#include "MSA_math.h"
#include "MSA_noise.h"

#define TABSIZE				256
#define TABMASK				(TABSIZE-1)
#define PERM(x)				perm[(x)&TABMASK]
#define INDEX2(ix,iy)		PERM((ix)+PERM(iy))
#define INDEX3(ix,iy,iz)	PERM((ix)+PERM((iy)+PERM(iz)))
//#define INDEX3(ix,iy,iz)	PERM((ix)+PERM(iy)+(iz))

static float *pfGradTab	=0;					// table of unit gradient vectors for gNoise
static float *pfValTab	=0;					// table of -1...1 values for vNoise

static unsigned char perm[TABSIZE] = {
        225,155,210,108,175,199,221,144,203,116, 70,213, 69,158, 33,252,
          5, 82,173,133,222,139,174, 27,  9, 71, 90,246, 75,130, 91,191,
        169,138,  2,151,194,235, 81,  7, 25,113,228,159,205,253,134,142,
        248, 65,224,217, 22,121,229, 63, 89,103, 96,104,156, 17,201,129,
         36,  8,165,110,237,117,231, 56,132,211,152, 20,181,111,239,218,
        170,163, 51,172,157, 47, 80,212,176,250, 87, 49, 99,242,136,189,
        162,115, 44, 43,124, 94,150, 16,141,247, 32, 10,198,223,255, 72,
         53,131, 84, 57,220,197, 58, 50,208, 11,241, 28,  3,192, 62,202,
         18,215,153, 24, 76, 41, 15,179, 39, 46, 55,  6,128,167, 23,188,
        106, 34,187,140,164, 73,112,182,244,195,227, 13, 35, 77,196,185,
         26,200,226,119, 31,123,168,125,249, 68,183,230,177,135,160,180,
         12,  1,243,148,102,166, 38,238,251, 37,240,126, 64, 74,161, 40,
        184,149,171,178,101, 66, 29, 59,146, 61,254,107, 42, 86,154,  4,
        236,232,120, 21,233,209, 45, 98,193,114, 78, 19,206, 14,118,127,
         48, 79,147, 85, 30,207,219, 54, 88,234,190,122, 95, 67,143,109,
        137,214,145, 93, 92,100,245,  0,216,186, 60, 83,105, 97,204, 52
};


static void GradTabInit()
{
    DEL(pfGradTab);
	pfGradTab=new float[TABSIZE*3];
	if(!pfGradTab) _FUCKER 
	float *table = pfGradTab;
	float z, r, theta;
	for(int c=0; c<TABSIZE; c++) {
		z = 1.0f - 2.0f*_Rand01;
		r = (float)sqrt(1-z*z);
		theta = 2 * _PI * _Rand01;
		*table++ = r * (float)cos(theta);
		*table++ = r * (float)sin(theta);
		*table++ = z;
	}
}


static void ValTabInit()
{
    DEL(pfValTab);
	pfValTab=new float[TABSIZE];
	if(!pfValTab) _FUCKER 
	float *table=pfValTab;
    for(int c=0; c<TABSIZE; c++) *table++=1.0f-2.0f*_Rand01;
}


void InitNoise(int iWhich, int iSeed)
{
	srand(iSeed);
	if(iWhich & _NOISE_GRAD)	
		GradTabInit();
	if(iWhich & _NOISE_VAL)		
		ValTabInit();
}



static inline float gLattice(int ix, int iy, float fx, float fy)
{
	return pfGradTab[INDEX2(ix,iy)*2+1]*fx + pfGradTab[INDEX2(ix,iy)*2+2]*fy;
}


static inline float gLattice(int ix, int iy, int iz,float fx, float fy, float fz)
{
    float *g = &pfGradTab[INDEX3(ix,iy,iz)*3];
    return g[0]*fx + g[1]*fy + g[2]*fz;
}


static inline float vLattice(int ix, int iy, int iz)
{
    return pfValTab[INDEX3(ix,iy,iz)];
}

static inline float mLattice(int ix, int iy, int iz,float fx, float fy, float fz)
{
    float fLen=(fx*fx+fy*fy+fz*fz);
/*	__asm {
		fld fLen
		fsqrt
		fstp fLen
	}*/
	fLen=0.6-fLen;
	if(fLen<0) 
		fLen=0;
	return pfValTab[INDEX3(ix,iy,iz)]*fLen;
}


float gNoise(float x, float y)
{
    int   ix = _Floor(x);
    float fx0 = x-ix;
    float fx1 = fx0-1;
    float wx = _Smooth01(fx0);
    
    int   iy = _Floor(y);
    float fy0 = y-iy;
    float fy1 = fy0-1;
    float wy = _Smooth01(fy0);
    
	float vx0,vx1,vy0,vy1;
    vx0 = gLattice(ix,iy,fx0,fy0);
    vx1 = gLattice(ix+1,iy,fx1,fy0);
    vy0 = _Lerp(vx0,vx1,wx);
    vx0 = gLattice(ix,iy+1,fx0,fy1);
    vx1 = gLattice(ix+1,iy+1,fx1,fy1);
    vy1 = _Lerp(vx0,vx1,wx);
 
    float fRet=_Lerp(vy0,vy1,wy);
	return fRet;
}


float gNoise(float x, float y, float z)
{
    int   ix = _Floor(x);
    float fx0 = x - ix;
    float fx1 = fx0 - 1;
    float wx = _Smooth01(fx0);

    int   iy = _Floor(y);
    float fy0 = y - iy;
    float fy1 = fy0 - 1;
    float wy = _Smooth01(fy0);

    int   iz = _Floor(z);
    float fz0 = z - iz;
    float fz1 = fz0 - 1;
    float wz = _Smooth01(fz0);

    float vx0, vx1, vy0, vy1, vz0, vz1;
    vx0 = gLattice(ix,iy,iz,fx0,fy0,fz0);
    vx1 = gLattice(ix+1,iy,iz,fx1,fy0,fz0);
    vy0 = _Lerp(vx0,vx1,wx);
    vx0 = gLattice(ix,iy+1,iz,fx0,fy1,fz0);
    vx1 = gLattice(ix+1,iy+1,iz,fx1,fy1,fz0);
    vy1 = _Lerp(vx0,vx1,wx);
    vz0 = _Lerp(vy0,vy1,wy);

    vx0 = gLattice(ix,iy,iz+1,fx0,fy0,fz1);
    vx1 = gLattice(ix+1,iy,iz+1,fx1,fy0,fz1);
    vy0 = _Lerp(vx0,vx1,wx);
    vx0 = gLattice(ix,iy+1,iz+1,fx0,fy1,fz1);
    vx1 = gLattice(ix+1,iy+1,iz+1,fx1,fy1,fz1);
    vy1 = _Lerp(vx0,vx1,wx);
    vz1 = _Lerp(vy0,vy1,wy);
	
	float fRet=_Lerp(vz0,vz1,wz);
	return fRet;
}



float MSANoise(float x, float y, float z)
{
    int   ix = _Floor(x);
    float fx0 = x - ix;
//  fx0	= _Smooth01(fx0);
    float fx1 = fx0 - 1;

    int   iy = _Floor(y);
    float fy0 = y - iy;
//  fy0	= _Smooth01(fy0);
    float fy1 = fy0 - 1;

    int   iz = _Floor(z);
    float fz0 = z - iz;
//  fz0	= _Smooth01(fz0);
    float fz1 = fz0 - 1;

    //float vx0, vx1, vy0, vy1, vz0, vz1;
	float fRet= (mLattice(ix,iy,iz,fx0,fy0,fz0)		+	mLattice(ix+1,iy,iz,fx1,fy0,fz0)  +
				mLattice(ix,iy+1,iz,fx0,fy1,fz0)	+	mLattice(ix+1,iy+1,iz,fx1,fy1,fz0)+
				mLattice(ix,iy,iz+1,fx0,fy0,fz1)	+	mLattice(ix+1,iy,iz+1,fx1,fy0,fz1)+
				mLattice(ix,iy+1,iz+1,fx0,fy1,fz1)	+	mLattice(ix+1,iy+1,iz+1,fx1,fy1,fz1));
	if(fRet>1) 
		fRet=0;
	else if(fRet<-1) 
		fRet=0;

	return fRet;
}


/*****************************************************************************************/
/*
float vnoise(float x, float y, float z)
{
    float xknots[4], yknots[4], zknots[4];

    int ix = FLOOR(x);
    float fx = x - ix;

    int iy = FLOOR(y);
    float fy = y - iy;

    int iz = FLOOR(z);
    float fz = z - iz;

    for (int k=-1; k<=2; k++) {
        for (int j=-1; j<=2; j++) {
            for (int i=-1; i<=2; i++) xknots[i+1]=vLattice(ix+i,iy+j,iz+k);
            yknots[j+1]=spline(fx,4,xknots);
        }
        zknots[k+1]=spline(fy,4,yknots);
    }
    return spline(fz,4,zknots);
}

/****************************************************************************************/
/*
float vcnoise(float x, float y, float z)
{
    int i, j, k;
    float dx, dy, dz;
    float sum = 0;

    int ix = FLOOR(x);
    float fx = x - ix;

    int iy = FLOOR(y);
    float fy = y - iy;

    int iz = FLOOR(z);
    float fz = z - iz;

    for (int k = -1; k <= 2; k++) {
        dz = k - fz;
        dz = dz*dz;
        for (int j = -1; j <= 2; j++) {
            dy = j - fy;
            dy = dy*dy;
            for (int i = -1; i <= 2; i++){
                dx = i - fx;
                dx = dx*dx;
                sum += vLattice(ix+i,iy+j,iz+k)
                    * catrom2(dx + dy + dz);
            }
        }
    }
    return sum;
}

/*****************************************************************************************/
/*
#define NEXT(h)     (((h)+1) & TABMASK)
#define NIMPULSES   3

static float impulseTab[TABSIZE*4];

static void impulseTabInit(int seed);

float
scnoise(float x, float y, float z)
{
    static int initialized;
    float *fp;
    int i, j, k, h, n;
    int ix, iy, iz;
    float sum = 0;
    float fx, fy, fz, dx, dy, dz, distsq;

    // Initialize the random impulse table if necessary. 
    if (!initialized) {
        impulseTabInit(665);
        initialized = 1;
    }

    ix = FLOOR(x); fx = x - ix;
    iy = FLOOR(y); fy = y - iy;
    iz = FLOOR(z); fz = z - iz;
    
    // Perform the sparse convolution. 
    for (i = -2; i <= 2; i++) {
      for (j = -2; j <= 2; j++) {
        for (k = -2; k <= 2; k++) {
            // Compute voxel hash code. 
            h = INDEX3(ix+i,iy+j,iz+k);
            
            for (n = NIMPULSES; n > 0; n--, h = NEXT(h)) {
                // Convolve filter and impulse. 
                fp = &impulseTab[h*4];
                dx = fx - (i + *fp++);
                dy = fy - (j + *fp++);
                dz = fz - (k + *fp++);
                distsq = dx*dx + dy*dy + dz*dz;
                sum += catrom2(distsq) * *fp;
            }
        }
      }
    }

    return sum / NIMPULSES;
}

static void
impulseTabInit(int seed)
{
    int i;
    float *f = impulseTab;

    srandom(seed); // Set random number generator seed. 
    for (i = 0; i < TABSIZE; i++) {
        *f++ = RANDNBR;
        *f++ = RANDNBR;
        *f++ = RANDNBR;
        *f++ = 1. - 2.*RANDNBR;
    }
}
*/
