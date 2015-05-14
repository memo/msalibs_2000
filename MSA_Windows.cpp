/**************************************************

	All functions dealing with standard Windows stuff


****************************************************/


#include "MSA_WinApp.h"


#define	WINDOWCLASS		"MSAClass"




int	MSA_WinUpdate(int width=0, int height=0);		// Called when Window is moved or resized


/************************ Window Procedure*********************************/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg) {
		
		case WM_SYSCOMMAND	:
			switch(wParam) {
				case SC_SCREENSAVE:		// If Screen Saver is starting
				case SC_MONITORPOWER:	// or Monitor entering PowerSave
				return 0;				// then Prevent and return
			}
			break;
		
		case WM_ACTIVATE	: AppInfo.bActive = (wParam == WA_ACTIVE) || (wParam == WA_CLICKACTIVE); return 0;
		case WM_CLOSE		: AppInfo.bQuit = TRUE; return 0;

		case WM_KEYDOWN		: MSA_SetButton(wParam, TRUE); return 0;
		case WM_KEYUP		: MSA_SetButton(wParam, FALSE); return 0;

		case WM_LBUTTONDOWN	:
        case WM_RBUTTONDOWN	:
        case WM_LBUTTONUP	:
        case WM_RBUTTONUP	:
//        case WM_MOUSEMOVE	:
			MSA_SetButton(MSA_MOUSE_LEFT,   wParam & MK_LBUTTON);
			MSA_SetButton(MSA_MOUSE_RIGHT,  wParam & MK_RBUTTON);
			MSA_SetButton(MSA_MOUSE_MIDDLE, wParam & MK_MBUTTON);
			
//            Inputs.i2Mouse.x     = LOWORD(lParam);
//            Inputs.i2Mouse.y     = HIWORD(lParam);
			return 0;;
  		
		case WM_SIZE		: MSA_WinUpdate( LOWORD(lParam), HIWORD(lParam) ); return 0;
		case WM_MOVE		: MSA_WinUpdate(); return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}





/************************ Create Window *********************************/
int MSA_CreateWindow()
{
	WNDCLASS	wc;
	DWORD		dwStyle;
	DWORD		dwExStyle;

	AppInfo.bFullScreen = InitInfo.bFullScreen;
	AppInfo.hInst		= InitInfo.hInst;

	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= AppInfo.hInst;
	wc.hIcon			= InitInfo.hIcon;
	wc.hCursor			= InitInfo.hCursor;
	wc.hbrBackground	= NULL;
	wc.lpszMenuName		= InitInfo.szMenu;
	wc.lpszClassName	= WINDOWCLASS;

	
	MSA_Check( RegisterClass(&wc), "ERROR : Registering Window Class", "MSA_CreateWindow" );

	// Change Displaymode if fullscreen
	if(AppInfo.bFullScreen) {
		DEVMODE		dmScreenSettings;
		ClearObject(dmScreenSettings);
		dmScreenSettings.dmSize			= sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth	= InitInfo.rcWindow.x;
		dmScreenSettings.dmPelsHeight	= InitInfo.rcWindow.y;
		dmScreenSettings.dmBitsPerPel	= InitInfo.iBitDepth;
		dmScreenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if(ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			if( Ask("Selected Fullscreen Mode is not Supported.\nUse windowed mode instead?") ) AppInfo.bFullScreen = FALSE;
			else return FALSE;
		}
	}


	// Set window properties
	if(AppInfo.bFullScreen) {		
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;								// no border
		ShowCursor(FALSE);
	} else {
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	// give 3D edge
		dwStyle = WS_OVERLAPPEDWINDOW;					// titlebar, sizing border, window menu and control box
	}
	dwStyle |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;		// required for OpenGL;


	RECT rcWindow = {0, 0, InitInfo.rcWindow.x, InitInfo.rcWindow.y};
	AdjustWindowRectEx(&rcWindow, dwStyle, FALSE, dwExStyle);	// Adjust window size to make clientsize requested size


	// Create the window
	MSA_Check( AppInfo.hWnd = CreateWindowEx( 
			dwExStyle,							// extended style
			WINDOWCLASS,						// window class name
			InitInfo.szTitle,					// title
			dwStyle,							// style
			CW_USEDEFAULT, CW_USEDEFAULT,		// position
			rcWindow.right - rcWindow.left,		// adjusted width
			rcWindow.bottom - rcWindow.top,		// adjusted height
			NULL,								// parent window
			NULL,								// menu
			AppInfo.hInst,						// Instance
			NULL), "ERROR : Creating Window", "MSA_CreateWindow" )



	ShowWindow(AppInfo.hWnd, InitInfo.iCmdShow);
			
	MSA_SetV2(Inputs.i2MouseScrOld, AppInfo.rcWindow.cx, AppInfo.rcWindow.cy);
	SetCursorPos(Inputs.i2MouseScrOld.x, Inputs.i2MouseScrOld.y);

	SetForegroundWindow(AppInfo.hWnd);
	SetFocus(AppInfo.hWnd);

	if(AppInfo.bFullScreen)
		SetWindowPos(AppInfo.hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	MSA_Return;
}




/****************************** Destroy Window *************************************/
int MSA_DestroyWindow()
{
	if(AppInfo.bFullScreen) {
		ChangeDisplaySettings(NULL, 0);		// Switch to desktop
		ShowCursor(TRUE);					// Show mouse
	}


	// Release DC
	if(AppInfo.hDC && ! ReleaseDC(AppInfo.hWnd, AppInfo.hDC)) {
		Err("ERROR : Releasing Device Context", "MSA_DestroyWindow");
		AppInfo.hDC = NULL;
	}

	// Release hWnd and destroy window
	if(AppInfo.hWnd) {
		if(!DestroyWindow(AppInfo.hWnd)) {
			Err("ERROR : Destroying Window", "MSA_DestroyWindow");
			AppInfo.hWnd = NULL;
		}

		if(!UnregisterClass(WINDOWCLASS, AppInfo.hInst)) {
			Err("ERROR : UnRegistering Window Class", "MSA_DestroyWindow");
			AppInfo.hInst = NULL;
		}
	}
	MSA_Return;
}
