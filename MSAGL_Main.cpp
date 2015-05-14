#include "MSAGL.h"



APPINFO		AppInfo;
INITINFO	InitInfo;
INPUTS		Inputs;



int		MSA_InitTimers();
int		MSA_KillTimers();

int		MSA_CreateWindow();								// Create a Window
int		MSA_DestroyWindow();							// Destroy a Window

int		MSA_CreateOpenGL();
int		MSA_DestroyOpenGL();								// Destroy OpenGL

void	MSA_Start();									// Defined by user in his app



//int		MSA_Destroy();




/************************************************************************************/
static int MSA_Init()
{
	ClearObject(AppInfo);
	ClearObject(Inputs);

	AppInfo.CallBacks = InitInfo.CallBacks;

	
	MSA_Check( MSA_CreateWindow(),		"ERROR : Creating Window",		"MSA_Init");

	AppInfo.bActive = FALSE;

	MSA_Check( MSA_InitTimers(),		"ERROR : Initailizing Timer",	"MSA_Init");
	MSA_Check( MSA_CreateOpenGL(),		"ERROR : Creating OpenGL Window",	"MSA_Init");
	MSA_Check( MSA_InitOpenGL(),		"ERROR : Initializing OpenGL",	"MSA_Init");
	
	MSA_CallBack(lpInit);

	AppInfo.bActive = TRUE;

	MSA_Return;
}



/************************************************************************************/
static int MSA_Destroy()
{
	AppInfo.bActive = FALSE;

	MSA_Check( MSA_KillTimers(),			"ERROR : Destroying Timer",			"MSA_Destroy");

	MSA_CallBack(lpDestroy);
	MSA_Check( MSA_DestroyOpenGL(),		"ERROR : Destroying OpenGL Window",	"MSA_Destroy");
	MSA_Check( MSA_DestroyWindow(),		"ERROR : Creating Window",			"MSA_Destroy");
	

	MSA_Return;
}



/************************************************************************************/
static inline int MSA_CheckInputs()
{
	GetCursorPos((POINT*)&Inputs.i2MouseScr); 
	
	Inputs.i2Mouse = Inputs.i2MouseScr;
	ScreenToClient(AppInfo.hWnd, ((POINT*)&Inputs.i2Mouse));

	Inputs.f2MouseSpeed.x = (Inputs.i2MouseScr.x - Inputs.i2MouseScrOld.x) * AppInfo.rcWindow.rx * AppInfo.fUpdateFreq;
	Inputs.f2MouseSpeed.y = (Inputs.i2MouseScr.y - Inputs.i2MouseScrOld.y) * AppInfo.rcWindow.ry * AppInfo.fUpdateFreq;


	if(Inputs.bLockMouse) SetCursorPos(Inputs.i2MouseScrOld.x, Inputs.i2MouseScrOld.y);
	else Inputs.i2MouseScrOld = Inputs.i2MouseScr;
	
	
	if(MSA_IsButtonNew('P')) AppInfo.bPause = !AppInfo.bPause;

	
	if(MSA_IsButtonNew(VK_F2)) {
	MSA_Check( MSA_Destroy(),	"ERROR : Destroying",	"MSA_CheckDefaultKeys");
		InitInfo.bFullScreen = !AppInfo.bFullScreen;
		MSA_Check( MSA_Init(),	"ERROR : Resizing",		"MSA_CheckDefaultKeys");
	}

	if(MSA_IsButtonNew(VK_ESCAPE)) AppInfo.bQuit = TRUE;
	
	MSA_Return;
}


int MainLoop_OnceEveryFrame();
int MainLoop_LoopEveryFrame();
int MainLoop_MultiThreaded();


/************************************************************************************/
int WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR szCmdLine, int nCmdShow)
{
	MSA_SetDefaults(hInst, szCmdLine, nCmdShow);	// Init InitInfo with defaults
	MSA_Start();									// Call user defined function to init InitInfo
	
	InitInfo.hInst = hInst;							// Make sure user hasnt overwritten some important stuff
	InitInfo.szCmdLine = szCmdLine;

	MSA_Check( MSA_InitMath(), "ERROR : Initializing Maths", "WinMain");	// Initialize Maths
	MSA_Check( MSA_Init(),	"ERROR : Initializing"		, "WinMain");	// Initialize Graphics

	int iRet;

	switch(InitInfo.LoopMode)
	{
		case MSA_OnceEveryFrame : iRet = MainLoop_OnceEveryFrame(); break;
		case MSA_LoopEveryFrame : iRet = MainLoop_LoopEveryFrame(); break;
		case MSA_MultiThreaded	: iRet = MainLoop_MultiThreaded();	break;
	}

	MSA_Check( MSA_Destroy(), "ERROR : Destroying"		, "WinMain");
	

	return iRet;
}



/************************************************************************************/
int MainLoop_OnceEveryFrame()
{
	MSG msg;
	while(!AppInfo.bQuit) {
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			AppInfo.bMessage = TRUE;
			if(msg.message == WM_QUIT)	AppInfo.bQuit = TRUE;
			else {	TranslateMessage(&msg);		DispatchMessage(&msg);	}
		} else {
			AppInfo.bMessage = FALSE;

			if(AppInfo.bActive) {
				MSA_CheckInputs();
				
				if(!AppInfo.bPause) {
					MSA_CallBack(lpLoop);
					
					MSA_Update();
				}

				MSA_Render();
			}
		}
	}
	return msg.wParam;
}



/************************************************************************************/
int MainLoop_MultiThreaded()
{
	MSG msg;
	while(!AppInfo.bQuit) {
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			AppInfo.bMessage = TRUE;
			if(msg.message == WM_QUIT)	AppInfo.bQuit = TRUE;
			else {	TranslateMessage(&msg);		DispatchMessage(&msg);	}
		} else {
			AppInfo.bMessage = FALSE;

			if(AppInfo.bActive) {
				MSA_CheckInputs();
				
				if(!AppInfo.bPause) {
					MSA_CallBack(lpLoop);
				}

				MSA_Render();
			}
		}
	}
	return msg.wParam;
}


/************************************************************************************/
int MainLoop_LoopEveryFrame()
{
	MSG msg;
	static DWORD dwLastTime = timeGetTime();

	while(!AppInfo.bQuit) {
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			AppInfo.bMessage = TRUE;
			if(msg.message == WM_QUIT)	AppInfo.bQuit = TRUE;
			else {	TranslateMessage(&msg);		DispatchMessage(&msg);	}
		} else {
			AppInfo.bMessage = FALSE;

			if(AppInfo.bActive) {
				MSA_CheckInputs();
				
				if(!AppInfo.bPause) {

					MSA_CallBack(lpLoop);
/*
					if(0) {
						DWORD dwNowTime = timeGetTime();
						DWORD dwDiff = dwNowTime - dwLastTime;

						if(dwDiff > AppInfo.wUpdatePeriod) {
							float fCount = (float)dwDiff / (float)AppInfo.wUpdatePeriod;
							for(int c=0; c<fCount; c++) {
								MSA_CallBack(lpUpdate);
								AppInfo.dwUpdateCount++;
							}
						}

						AppInfo.fSecs = dwDiff * 0.001f;
						AppInfo.fTime += AppInfo.fSecs;
						dwLastTime = dwNowTime;

					} else 
*/					{
						DWORD dwNowTime = timeGetTime();
						DWORD dwDiff = dwNowTime - dwLastTime;

						AppInfo.fSecs = dwDiff * 0.001f;
						AppInfo.fTime += AppInfo.fSecs;

						BOOL bLoop;
						do {
							if(dwDiff > AppInfo.wUpdatePeriod) {
								MSA_CallBack(lpUpdate);
								AppInfo.dwUpdateCount++;
								dwDiff -= AppInfo.wUpdatePeriod;
								bLoop = TRUE;
							} else {
								bLoop = FALSE;
							}

						} while(bLoop);

						dwLastTime = dwNowTime;
					}
				}

				MSA_Render();
			}
		}
	}
	return msg.wParam;
}
