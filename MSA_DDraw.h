#ifndef __MSA_rules_ye_DDRAW__
#define __MSA_rules_ye_DDRAW__

#include <ddraw.h>
#include "..\_MSA_Win\MSA_Win.h"


#define _DDErrorCheck(expr,str)		_ErrorCheck(expr==DD_OK, str, TheApp)

#define DD_PRIMARY	1
#define DD_BACK		2
#define DD_MEMORY	3

#define DD_WINDOW	true
#define DD_FULLSCR	false

class MSA_DDRAW 
{
	protected:

		#ifdef __with_sprites
			friend class MSA_SPRITE;
			friend class MSA_WORLD2D;
		#endif

		LPDIRECTDRAW4			pDD4;
		LPDIRECTDRAWSURFACE4	pDDS4_Prim;
		LPDIRECTDRAWSURFACE4	pDDS4_Back;
				
		MSA_WINAPP *TheApp;
		HRESULT (*InitExtraSurfaces)();

		void Init();
		void Destroy();
		
		void ReleaseSurfaces() ;
	public:
		MSA_DDRAW();
		MSA_DDRAW(MSA_WINAPP* tApp);
		~MSA_DDRAW();

		bool bWindow;
		DDSURFACEDESC2 ddsd2;

		HRESULT InitDDraw();
		HRESULT InitSurfaces(bool tbWindow=DD_FULLSCR, long tScrX=640, long tScrY=480, long tBitDepth=32);
		void SetInitFunc(HRESULT (*tInitExtraSurfaces)());

		HRESULT ChangeCoopLevel();

		LPDIRECTDRAWSURFACE4 GetSurface(int s=DD_BACK);

		void Flip();
		void FlipWait();
		void Clear(int Color=0);

		void AfterWinPaint();

		inline void Error(LPCSTR szTitle, HRESULT hRet);
};


#endif //__MSA_rules_ye_DDRAW__