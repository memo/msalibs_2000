#include <assert.h>

#include "ddutil.h"
#include "..\_MSA_Utils\MSA_Utils.h"
#include "MSA_world2d.h"

MSA_WORLD2D::MSA_WORLD2D()
{
	Init();
}


MSA_WORLD2D::MSA_WORLD2D(MSA_DDRAW *tTheDDraw)
{
	Init();
	TheDDraw=tTheDDraw;
	assert(DDSetColorKey(TheDDraw->pDDS4_Back, RGB(0,0,0))==DD_OK); 
}


MSA_WORLD2D::~MSA_WORLD2D()
{
	Destroy();
}


void MSA_WORLD2D::Init()
{		
//	_ClearMe;
	TheDDraw = NULL;
	spr = NULL;
	SpriteCount = NULL;

}


void MSA_WORLD2D::Destroy()
{
//	_del(spr);
	MSA_SPRITE *n=spr,*m;
	while (n) { m=n->Next; n->Next=0; n=m;}
}


void MSA_WORLD2D::WriteText(int x,int y,LPCTSTR lpString)
{
    HDC         hDC;
    if (TheDDraw->pDDS4_Back->GetDC(&hDC) == DD_OK) {
        SetBkColor(hDC, RGB(0, 0, 0));
        SetTextColor(hDC, RGB(255, 255, 255));
        TextOut(hDC, x, y, lpString, strlen(lpString));
        TheDDraw->pDDS4_Back->ReleaseDC(hDC);
    }
}


void MSA_WORLD2D::PutPixel(int x,int y,int c)
{
    HDC         hDC;
    if (TheDDraw->pDDS4_Back->GetDC(&hDC) == DD_OK)
    {
		SetPixel(hDC,x,y,c);
        TheDDraw->pDDS4_Back->ReleaseDC(hDC);
    }
/*	if(TheDDraw->pDDS4_Back->Lock(NULL,LPDDSURFACEDESC2 ddsd2,DDLOCK_SURFACEMEMORYPTR,NULL) == DD_OK) {
		ddsd2->lpSurface;

		TheDDraw->pDDS4_Back->Unlock(NULL);
	}
*/}


MSA_SPRITE* MSA_WORLD2D::AddSprite(MSA_SPRITE* ss)
{
	MSA_SPRITE* n=spr;
	if (SpriteCount<MAX_SPRITE_COUNT) {
		if(!ss) ss=new MSA_SPRITE;
		if(!spr) {
			spr=ss;
			n=spr;
		} else {
			while (n->Next) n=n->Next;
			n->Next=ss;
			n=n->Next;
		}
		n->World2D=this;
		n->iIndex=SpriteCount;
		SpriteCount++;
	}
	return n;
}


MSA_SPRITE* MSA_WORLD2D::AddSprite(LPCSTR szBitmap)
{
	MSA_SPRITE* n=AddSprite();
	if(n) n->Load(szBitmap);
	return n;
}


void MSA_WORLD2D::DeleteAllSprites()
{
	DEL(spr);
}


MSA_SPRITE* MSA_WORLD2D::Sprite(short i)
{ 
	MSA_SPRITE* n=spr;
	for (short c=0; c<i; c++) n=n->Next;
	return n;
}


void MSA_WORLD2D::Clear(int Color)
{
	TheDDraw->Clear(Color);
}


void MSA_WORLD2D::Update()
{
	if(!TheDDraw->TheApp->bPaused) {
		MSA_SPRITE* n=spr;
		while (n) { n->Update(); n=n->Next;}
	}
}


void MSA_WORLD2D::Render()
{
	MSA_SPRITE* n=spr;
	while (n) { n->Render(); n=n->Next;	}
}

