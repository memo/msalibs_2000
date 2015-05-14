#include <assert.h>

#include "ddutil.h"
#include "..\_MSA_Utils\MSA_Utils.h"
#include "..\_MSA_Utils\MSA_Math.h"
#include "MSA_sprite.h"


MSA_SPRITE::MSA_SPRITE (MSA_WORLD2D *tw,LPCSTR szBitmap)
{
	Init();
	if(tw) tw->AddSprite(this);
	if(szBitmap) Load(szBitmap);
}


MSA_SPRITE::~MSA_SPRITE()									
{
	Destroy();
}


void MSA_SPRITE::Init()
{
	iIndex = 0;
	szName = NULL;
	
	World2D = NULL;
	Next = NULL;
	Parent = NULL;
	dwState = 0;
	CLEARTHIS(&rcDest);

	WorldInfo=new WORLDINFO; CLEARTHIS(WorldInfo);

	BmpInfo = new BMPINFO; CLEARTHIS(BmpInfo);

	CLEARTHIS(&AnimInfo);

	SetFlags(DDBLT_KEYSRC);
	BltInfo.pDDBltFx=new DDBLTFX; CLEARTHIS(BltInfo.pDDBltFx);
	BltInfo.pDDBltFx->dwSize = sizeof(DDBLTFX);

	SetFrameCount(1);
	SetAnimCount(1);
	SetAnimSeg(1);
	SetFrame(0);
	SetFPS(0);
}


void MSA_SPRITE::Destroy()
{
	if(WorldInfo) {
	}
	DEL(WorldInfo);

	if(!CHKFLAG(dwState,_spr_LinkedTexture) && BmpInfo) {
		REL(BmpInfo->pDDS4_bm);
		DEL(BmpInfo->rcSource);
		DEL(BmpInfo->ptOrigin);
		DEL(BmpInfo->ptDim);
		DEL(BmpInfo->ptAnim);
		DEL(BmpInfo);
	}

	DEL(BltInfo.pDDBltFx);
	DEL(szName);
	DEL(Next);

}


void MSA_SPRITE::SetFlags(DWORD tdwFlags)
{
	BltInfo.dwFlags=tdwFlags|DDBLT_ASYNC;
}


void MSA_SPRITE::AddFlags(DWORD tdwFlags)
{
	BltInfo.dwFlags|=tdwFlags;
}

void MSA_SPRITE::SetName(LPCSTR szN)
{
	DEL(szName);
	if(szN) {
		szName = new char[strlen(szN)+1];
		strcpy(szName,szN);
	}
}

int MSA_SPRITE::Load(LPCSTR szBitmap, bool bInit)
{
	REL(BmpInfo->pDDS4_bm);

	BmpInfo->pDDS4_bm = DDLoadBitmap(World2D->TheDDraw->pDD4,szBitmap,0,0);
	_ErrorCheck(BmpInfo->pDDS4_bm,szBitmap,World2D->TheDDraw->TheApp);

	DDSURFACEDESC2 ddsd;
	ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
    BmpInfo->pDDS4_bm->GetSurfaceDesc(&ddsd);
	BmpInfo->ptBmpSize.x=ddsd.dwWidth;
	BmpInfo->ptBmpSize.y=ddsd.dwHeight;
	if(bInit) {
		SetName(szBitmap);
		InitFrameInfo(0,0,0,BmpInfo->ptBmpSize.x,BmpInfo->ptBmpSize.y);
	}
	assert(DDSetColorKey(BmpInfo->pDDS4_bm, RGB(0,0,0))==DD_OK); 
	UpdateRect();
	return 1;
}


void MSA_SPRITE::AffectSource(DWORD dwDDFX)
{
	if(dwDDFX) {
		DWORD dwFlags=DDBLT_DDFX;
		DDBLTFX DDBltFx;
		CLEARTHIS(&DDBltFx);
		DDBltFx.dwSize = sizeof(DDBLTFX);
		DDBltFx.dwDDFX = dwDDFX;
		RECT rcTemp={0,0,BmpInfo->ptBmpSize.x,BmpInfo->ptBmpSize.y};

		while (1) {
			HRESULT hRet=BmpInfo->pDDS4_bm->Blt(&rcTemp ,BmpInfo->pDDS4_bm, &rcTemp, dwFlags, &DDBltFx);
			if (hRet == DD_OK ) break;
			if (hRet == DDERR_SURFACELOST ) {
				hRet = World2D->TheDDraw->pDDS4_Back->Restore();
				if (hRet != DD_OK) break;
			}
			if (hRet != DDERR_WASSTILLDRAWING) break;
		}
	}
}

void MSA_SPRITE::LinkTo(MSA_SPRITE *sprParent)
{
	Parent=sprParent;
}


void MSA_SPRITE::LinkTexture(MSA_SPRITE *sprParent)
{
	SETFLAG(dwState,_spr_LinkedTexture);
	BmpInfo=sprParent->BmpInfo;
}


void MSA_SPRITE::SetFPS(int tFPS)
{
	AnimInfo.iAnimInt = tFPS ? (100/_Abs(tFPS)) : 10000000;
}

void MSA_SPRITE::MulSpeed(float fMul)
{
	AnimInfo.iAnimInt = (int)(AnimInfo.iAnimInt/fMul);
}


void MSA_SPRITE::SetFrameCount(int tFC)
{
	BmpInfo->iFrameCount=tFC;
	DEL(BmpInfo->rcSource);		BmpInfo->rcSource=new RECT[tFC];
	DEL(BmpInfo->ptOrigin);		BmpInfo->ptOrigin=new POINT[tFC];
	DEL(BmpInfo->ptDim);		BmpInfo->ptDim=new POINT[tFC];
}


void MSA_SPRITE::InitFrameInfo(int tFNo,int left,int top,int width,int height,float tox,float toy)
{
	SetRect((BmpInfo->rcSource+tFNo),left,top,left+width,top+height);
	BmpInfo->ptDim[tFNo].x = width;
	BmpInfo->ptDim[tFNo].y = height;
	BmpInfo->ptOrigin[tFNo].x = (long)(tox * BmpInfo->ptDim[tFNo].x);
	BmpInfo->ptOrigin[tFNo].y = (long)(toy * BmpInfo->ptDim[tFNo].y);
}


void MSA_SPRITE::InitFrameInfoMxN(int m, int n,float tox,float toy)
{
	int sx=BmpInfo->ptBmpSize.x/m;
	int sy=BmpInfo->ptBmpSize.y/n;
	SetFrameCount(m*n);
	for(int j=0; j<n; j++) {
		for(int i=0; i<m; i++) {
			InitFrameInfo(j*m+i,i*sx,j*sy,sx,sy,tox,toy);
		}
	}
}


void MSA_SPRITE::UpdateRect()
{
	int iFC=BmpInfo->ptAnim[AnimInfo.iAnimNo].y - BmpInfo->ptAnim[AnimInfo.iAnimNo].x + 1;
	if(AnimInfo.iFrameNo<BmpInfo->ptAnim[AnimInfo.iAnimNo].x) AnimInfo.iFrameNo+=iFC;
	else if(AnimInfo.iFrameNo>BmpInfo->ptAnim[AnimInfo.iAnimNo].y) AnimInfo.iFrameNo-=iFC;
	
	MSA_COMPLEX TempPos=WorldInfo->Pos;

	if(Parent) TempPos+=Parent->WorldInfo->Pos;
	SetRect(&rcDest, 
		(int)TempPos.x - BmpInfo->ptOrigin[AnimInfo.iFrameNo].x, 
		(int)TempPos.y - BmpInfo->ptOrigin[AnimInfo.iFrameNo].y, 
		(int)TempPos.x + BmpInfo->ptDim[AnimInfo.iFrameNo].x - BmpInfo->ptOrigin[AnimInfo.iFrameNo].x, 
		(int)TempPos.y + BmpInfo->ptDim[AnimInfo.iFrameNo].y - BmpInfo->ptOrigin[AnimInfo.iFrameNo].y); 
}


void MSA_SPRITE::SetFrame(int tFNo)
{
	AnimInfo.iFrameNo=tFNo;
	UpdateRect();
}


void MSA_SPRITE::IncFrame(int tFd)
{
	AnimInfo.iFrameNo+=tFd;
	UpdateRect();
}


int MSA_SPRITE::GetFrame()
{
	return AnimInfo.iFrameNo;
}


void MSA_SPRITE::SetAnimCount(int tAC)
{
	BmpInfo->iAnimCount=tAC;
	AnimInfo.iAnimNo=0;
	DEL(BmpInfo->ptAnim); BmpInfo->ptAnim=new POINT[tAC];
	InitAnimSeg(0,0,BmpInfo->iFrameCount-1);
	SetAnimSeg(0);
}


void MSA_SPRITE::InitAnimSeg(int itANo,int iMin,int iMax)
{
//	if(itANo>=BmpInfo->iAnimCount) itANo=BmpInfo->iAnimCount-1;
	BmpInfo->ptAnim[itANo].x=iMin;
	BmpInfo->ptAnim[itANo].y=iMax;
}


void MSA_SPRITE::SetAnimSeg(int itAnimNo)
{
	AnimInfo.iAnimNo=itAnimNo;
	if(AnimInfo.iAnimNo>=BmpInfo->iAnimCount) AnimInfo.iAnimNo=BmpInfo->iAnimCount-1;
	UpdateRect();
}


void MSA_SPRITE::SetLimits(int x1,int y1,int w,int h)
{
	if(w && h) {
		SETFLAG(dwState,_spr_Physics);
		SetRect(&WorldInfo->rcLimits,x1,y1,x1+w,y1+h);
	} else {
		SetRectEmpty(&WorldInfo->rcLimits);
	}
}


void MSA_SPRITE::SetPos(float tx, float ty)					
{ 
	WorldInfo->Pos.x=tx; WorldInfo->Pos.y=ty;
	UpdateRect();
}


void MSA_SPRITE::IncPos(float dx, float dy)					
{	
	WorldInfo->Pos.x+=dx;
	WorldInfo->Pos.y+=dy;
	UpdateRect();
}


void MSA_SPRITE::SetSpeed(float tsx, float tsy)					
{	
	SETFLAG(dwState,_spr_Physics);
	WorldInfo->Speed.x=tsx; 
	WorldInfo->Speed.y=tsy; 
}


void MSA_SPRITE::IncSpeed(float tsx,float tsy)
{
	SETFLAG(dwState,_spr_Physics);
	WorldInfo->Speed.x+=tsx;
	WorldInfo->Speed.y+=tsy;
}


void MSA_SPRITE::ClampSpeed()
{
	float SpeedR=(float)(sqrt(WorldInfo->Speed.x*WorldInfo->Speed.x+WorldInfo->Speed.y*WorldInfo->Speed.y));
	float Theta=(float)(atan2(WorldInfo->Speed.y,WorldInfo->Speed.x));
	if(SpeedR>WorldInfo->SpeedLimits.y) SpeedR=WorldInfo->SpeedLimits.y;
	else if(SpeedR<WorldInfo->SpeedLimits.x) SpeedR=WorldInfo->SpeedLimits.x;
	SetSpeed(SpeedR*(float)cos(Theta),SpeedR*(float)sin(Theta));
}


void MSA_SPRITE::SetSpeedLimits(float tfMin,float tfMax)
{
	SETFLAG(dwState,_spr_Physics);
	WorldInfo->SpeedLimits.x=tfMin;
	WorldInfo->SpeedLimits.y=tfMax;
}


void MSA_SPRITE::Update()
{
	if(Parent) UpdateRect();
	else if(CHKFLAG(dwState,_spr_Physics)) {
		if(WorldInfo->fGravity) IncSpeed(0,WorldInfo->fGravity);
		if(WorldInfo->SpeedLimits.y) ClampSpeed();
		IncPos(WorldInfo->Speed.x,WorldInfo->Speed.y);
		if(WorldInfo->rcLimits.right || WorldInfo->rcLimits.bottom) {
			if(rcDest.left < WorldInfo->rcLimits.left)	{ 
				IncPos((float)(WorldInfo->rcLimits.left-rcDest.left),0.0f);		
				SetSpeed(WorldInfo->fDamping * _Abs(WorldInfo->Speed.x), WorldInfo->Speed.y); 
			}
			if(rcDest.right > WorldInfo->rcLimits.right) { 
				IncPos((float)(WorldInfo->rcLimits.right-rcDest.right),0.0f);
				SetSpeed(-WorldInfo->fDamping * _Abs(WorldInfo->Speed.x), WorldInfo->Speed.y); 
			}
			if(rcDest.top < WorldInfo->rcLimits.top) { 
				IncPos(0.0f,(float)(WorldInfo->rcLimits.top-rcDest.top));
				SetSpeed(WorldInfo->Speed.x, WorldInfo->fDamping * _Abs(WorldInfo->Speed.y)); 
			}
			if(rcDest.bottom > WorldInfo->rcLimits.bottom)	{ 
				IncPos(0.0f,(float)(WorldInfo->rcLimits.bottom-rcDest.bottom));
				SetSpeed(WorldInfo->Speed.x, -WorldInfo->fDamping * _Abs(WorldInfo->Speed.y)); 
			}
		}
	}

	if(BmpInfo->iFrameCount>1) {
		AnimInfo.iCounter++;
		if(AnimInfo.iCounter>=AnimInfo.iAnimInt) {
			IncFrame();
			AnimInfo.iCounter=1;
		}
	}
}


void MSA_SPRITE::Render()
{
	HRESULT hRet;
	while (1) {
		hRet=World2D->TheDDraw->pDDS4_Back->Blt(&rcDest ,BmpInfo->pDDS4_bm, (BmpInfo->rcSource+AnimInfo.iFrameNo), BltInfo.dwFlags, BltInfo.pDDBltFx);
//		hRet=World2D->TheDDraw->pDDS4_Back->Blt(&rcDest ,BmpInfo->pDDS4_bm, (BmpInfo->rcSource+AnimInfo.iFrameNo), BltInfo.dwFlags | DDBLT_WAIT, BltInfo.pDDBltFx);
//		hRet=World2D->TheDDraw->pDDS4_Back->BltFast(rcDest.left,rcDest.top, BmpInfo->pDDS4_bm,BmpInfo->rcSource+AnimInfo.iFrameNo, DDBLTFAST_SRCCOLORKEY);
		if (hRet == DD_OK ) break;
		if (hRet == DDERR_SURFACELOST ) {
			hRet = World2D->TheDDraw->pDDS4_Back->Restore();
			if (hRet != DD_OK) break;
		}
		if (hRet != DDERR_WASSTILLDRAWING) break;
	}
//	*/
}
