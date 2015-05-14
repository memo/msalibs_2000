#ifndef __MSA_rules_ye_Windows__
#define __MSA_rules_ye_Windows__

#include <windows.h>
#include <stdio.h>



#include "MSA_Input.h"
#include "MSA_Math.h"
#include "MSA_Utils.h"
#include "MSA_Globals.h"

#include <mmsystem.h>
#pragma comment (lib, "winmm.lib")




/*************************************************************************************/
typedef struct { float r, g, b, a; }	MSA_RGBA;

#define MSA_SetRGBA(vv,R,G,B,A)	{ (vv).r = R; (vv).g = G; (vv).b = B; (vv).a = (A); }




typedef struct { 
	int x1, y1, x2, y2;			// upper left, lower right 
	int x, y;					// width and height
	int cx, cy;					// center pos
	float rx, ry;				// reciprocal of width and height
	float ar;					// aspect ratio
} MSA_RECT;


void __inline __fastcall MSA_SetRect(MSA_RECT &vv, int X = 0, int Y = 0)
{
	if(X) {
		vv.x = X;				vv.y = Y;
		vv.x2 = vv.x1 + X;		vv.y2 = vv.y1 + Y;
		vv.rx = 1.0f / X;		vv.ry = 1.0f / Y;
		vv.ar = (float)(X)/(float)(Y); 
	}
	vv.cx = (vv.x1 + vv.x2) / 2; 	vv.cy = (vv.y1 + vv.y2) / 2; 
}


//#define MSA_SetRect(vv,X1,Y1,X2,Y2)	{ vv.x1 = X1;  vv.y1 = Y1; MSA_SetRectSize(vv, (X2-X1), (Y2-Y1) ); }




/************************* Callback Functions ****************************************/
typedef struct {
	int			(*lpInit)();			// Callback function for Initializing app
	int			(*lpDestroy)();			// Callback function for Destroying app
	int			(*lpLoop)();			// Callback function called before everything once every loop
	int			(*lpUpdate)();			// Callback function called every wUpdatePeriod
	int			(*lpRender)();			// Callback function for rendering

	int			(*lpWinUpdate)();		// Callback function for when window gets resized
} CALLBACKS;



#define MSA_SetInitScene(lpFunc)	InitInfo.CallBacks.lpInit		= lpFunc;
#define MSA_SetDestroy(lpFunc)		InitInfo.CallBacks.lpDestroy	= lpFunc;
#define MSA_SetLoop(lpFunc)			InitInfo.CallBacks.lpLoop		= lpFunc;
#define MSA_SetUpdate(lpFunc)		InitInfo.CallBacks.lpUpdate		= lpFunc;
#define MSA_SetRender(lpFunc)		InitInfo.CallBacks.lpRender		= lpFunc;

#define MSA_SetWinUpdate(lpFunc)	InitInfo.CallBacks.lpWinUpdate	= lpFunc;




enum LOOPMODE { MSA_OnceEveryFrame, MSA_LoopEveryFrame, MSA_MultiThreaded };

/********************* Structure to fill to initialize application ******************/
typedef struct __INITINFO {
	HINSTANCE		hInst;					// Instance
	LPSTR			szCmdLine;				// Command Line
	int				iCmdShow;				// How to Display Window (initialized from main func)
	
	MSA_RGBA		BackCol;				// MSA_RGBA with background information RGBA

	char			*szTitle;				// Title of window
	MSA_RECT		rcWindow;				// Dimesions of window
	int				iBitDepth;				// BitDepth
	BOOL			bFullScreen;			// Fullscreen or not;
	DWORD			dwPixelFlags;			// Flags for pixel info when creating PIXELFORMATDESCRIPTOR

	HICON			hIcon;					// Handle to Icon
	HCURSOR			hCursor;				// Handle to Cursor
	LPCTSTR			szMenu;					// MenuName

	LOOPMODE		LoopMode;				// MSA_OnceEveryFrame, MSA_LoopEveryFrame or MSA_MultiThreaded
	float			fUpdateFreq;			// Number of calls per second, ignored if LoopMode = MSA_OnceEveryFrame
	
	CALLBACKS		CallBacks;				// Callback Functions

} INITINFO;



void __inline __fastcall MSA_SetDefaults(HINSTANCE hInst, LPSTR szCmdLine, int nCmdShow)
{
	ClearObject(InitInfo);

	InitInfo.iCmdShow		= nCmdShow;
	InitInfo.hInst			= hInst;
	InitInfo.szCmdLine		= szCmdLine;

/*
	InitInfo.rcWindow.x		= 640;
	InitInfo.rcWindow.y		= 480;
	InitInfo.iBitDepth		= 16;
	InitInfo.bFullScreen	= FALSE;
	InitInfo.dwPixelFlags	= PFD_DOUBLEBUFFER;

	static char szTitle[] = "ye High 1 was here...";
	InitInfo.szTitle		= szTitle;
	InitInfo.fUpdateFreq	= 100;

	InitInfo.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	InitInfo.hCursor		= LoadCursor(NULL, IDC_ARROW);
*/
}






/***************************** All global runtime information *******************************/
typedef struct __APPINFO {
	HDC				hDC;					// Device Context of window
	HGLRC			hRC;					// Rendering Context of window
	HWND			hWnd;					// Handle to Window
	HINSTANCE		hInst;					// Handle to Instance of module

	MSA_RECT		rcWindow;				// Current size and dim of window
	MSA_RECT		rcScreen;				// Current size and dim of screen

	int				iTimeRes;				// Resolution of Timer
	WORD			wFPSTimerID;			// ID of Timer for FPS
	WORD			wTimerID;				// ID of main Timer
	float			fUpdateFreq;			// Number of calls per second
	WORD			wUpdatePeriod;			// ms between Update calls (1000 / wUpdateFreq)

	DWORD			dwRenderCount;			// Number of frames rendered since last render
	DWORD			dwUpdateCount;			// Number of updates called since last update
	DWORD			dwRenderFPS;			// FPS;
	DWORD			dwUpdateFPS;			// FPS;
	float			fSecs;					// Time elapsed since last call
	float			fTime;					// Time elapsed since beginning of timer started

	BOOL			bActive;				// Is application active
	BOOL			bPause;
	BOOL			bQuit;					// Does application wanna quit
	BOOL			bFullScreen;			// Is application fullscreen
	BOOL			bMessage;				// Messages being processed

	DWORD			dwError;				// No of errors occured

	char			szFlags[20];
	char			szDebugInfo[80];

	CALLBACKS		CallBacks;				// Callback Functions

} APPINFO;


/******************************** Prototypes ***************************************/


void __inline __fastcall MSA_Update()
{
	static DWORD dwLastTime = timeGetTime();

	DWORD dwNowTime = timeGetTime();

	AppInfo.fSecs = (dwNowTime - dwLastTime) * 0.001f;
	AppInfo.fTime += AppInfo.fSecs;

	dwLastTime = dwNowTime;
	MSA_CallBack(lpUpdate);
	AppInfo.dwUpdateCount++;
}




#endif	// __MSA_rules_ye_Windows__
