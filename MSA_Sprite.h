#ifndef __MSA_rules_ye_SPRITE__
#define __MSA_rules_ye_SPRITE__

#include "..\_MSA_Complex\MSA_Complex.h"
#include "MSA_world2d.h"

#define		_spr_Physics		1
#define		_spr_LinkedTexture	2

class MSA_SPRITE 
{
	protected:
		friend class MSA_WORLD2D;

		MSA_WORLD2D		*World2D;			// Pointer to owner World object
		MSA_SPRITE		*Next;				// pointer to next SPRITE for linked list
		MSA_SPRITE		*Parent;			// pointer to parent SPRITE (NULL for none)
		RECT			rcDest;				// destination rectangle on drawing surface
		DWORD			dwState;			// flags for state of sprite

		struct WORLDINFO {
			MSA_COMPLEX Pos;				// screen coordinates for origin of sprite;
			MSA_COMPLEX Speed;				// speed in x,y OR homein target x,y based on :
			MSA_COMPLEX SpeedLimits;		// Maximum and Minimum speeds (x min, y max)
			RECT		rcLimits;			// Pointer to Boundaries Sprite can go
			float		fGravity;			// gravity
			float		fDamping;			// bounciness off boundaries
		} *WorldInfo;

		
		struct BMPINFO {
			IDirectDrawSurface4 *pDDS4_bm;	// offscreen surface containing images
			POINT		ptBmpSize;			// width and height of bitmap
			RECT		*rcSource;			// list of source rectangles of each frame on offscreen surface
			POINT		*ptOrigin;			// origin of each frame rel to upper left corner;
			POINT		*ptDim;				// dimensions of each frame;
			POINT		*ptAnim;			// list of all Animation segments, x=>lower frame, y=>upper frame
			int			iFrameCount;		// Total number of frames
			int			iAnimCount;			// Total number of Animation Segments
		} *BmpInfo;


		struct {
			int			iAnimInt;			// Interval between frames in 10ms (1/fps)
			int			iFrameNo;			// Current frame no
			int			iAnimNo;			// Current seg no
			int			iCounter;			// Internal Counter for frame animation
		} AnimInfo; 
		
		
		struct {
			DWORD		dwFlags;			// flags for blt process
			DDBLTFX		*pDDBltFx;			// pointer to struct for blt process
		} BltInfo;

		
		virtual void Init();
		virtual void Destroy();

		inline void	UpdateRect();			// updates rcDest with current coor and frameno 
		inline void	ClampSpeed();

		virtual void Update();
		void		Render();

	public:
		int				iIndex;				// Index in the World2D linked list
		char			*szName;			// Name of sprite

		MSA_SPRITE(MSA_WORLD2D *tw = NULL, LPCSTR szBitmap = NULL);
		~MSA_SPRITE();

		void		SetFlags(DWORD tdwFlags = 0);
		void		AddFlags(DWORD tdwFlags);

		void		SetName(LPCSTR szN = NULL);
		int			Load(LPCSTR szBitmap, bool bInit=true);
		void		AffectSource(DWORD dwDDFX);
		void		LinkTo(MSA_SPRITE *sprParent = NULL);
		void		LinkTexture(MSA_SPRITE *sprParent = NULL);

		void		SetFPS(int tFPS);
		void		MulSpeed(float fMul);
		void		SetFrameCount(int tFC);
		void		InitFrameInfo(int FNo,int left,int top,int width,int height,float tox = 0.5f,float toy = 0.5f);
		void		InitFrameInfoMxN(int m, int n,float tox = 0.5f,float toy = 0.5f);
		void		SetFrame(int tFNo);
		void		IncFrame(int tFd=1);
		int			GetFrame();

		void		SetAnimCount(int tAC=1);
		void		InitAnimSeg(int itANo,int iMin,int iMax);
		void		SetAnimSeg(int itAnimNo);

		void		SetPos(float tx, float ty);
		void		IncPos(float dx, float dy);
		void		SetPos(MSA_COMPLEX& tc)			{ SetPos(tc.x,tc.y); }
		void		IncPos(MSA_COMPLEX& tc)			{ IncPos(tc.x,tc.y); }
		void		SetLimits(int x1=0,int y1=0,int w=0,int h=0);

		void		SetSpeed(float tsx=0,float tsy=0);
		void		IncSpeed(float tsx,float tsy);
		void		SetSpeed(MSA_COMPLEX& tc)		{ SetSpeed(tc.x,tc.y); }
		void		IncSpeed(MSA_COMPLEX& tc)		{ IncSpeed(tc.x,tc.y); }
		void		SetSpeedLimits(float tfMin=0,float tfMax=0);
};

#endif	// __MSA_rules_ye_SPRITE__