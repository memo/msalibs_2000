/**************************************************

	All functions dealing with OpenGL & Windows stuff


****************************************************/



#include "MSAGL.h"




/*********************** Window  Resized ***************************************/
int MSA_WinUpdate(int width, int height)
{
	MSA_SetRect(AppInfo.rcScreen, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	
	if(!AppInfo.bFullScreen) {
		MSA_Check( GetClientRect(AppInfo.hWnd, (RECT*) &AppInfo.rcWindow),			"ERROR : Getting Client Rect",				"MSA_WinUpdate");

		MSA_Check( ClientToScreen(AppInfo.hWnd, (POINT*)&(AppInfo.rcWindow.x1)),	"ERROR : Converting UpperLeft to Screen",	"MSA_WinUpdate");
		MSA_Check( ClientToScreen(AppInfo.hWnd, (POINT*)&(AppInfo.rcWindow.x2)),	"ERROR : Converting LowerRight to Screen",	"MSA_WinUpdate");
	}

	MSA_SetRect(AppInfo.rcWindow);

	if(width) {
		if(height==0) height = 1;

		MSA_SetRect(AppInfo.rcWindow, width, height);

		glViewport(0,0,width,height);
		Camera.bProjChanged = TRUE;

		MSA_CallBack(lpWinUpdate);

	}
	
	MSA_Render();

	MSA_Return;
}




/*********************** Create OpenGL in Window ***************************************/
int MSA_CreateOpenGL()
{
	// Get a Device Context
	MSA_Check( AppInfo.hWnd,						"ERROR : Window not Created",			"MSA_CreateOpenGL" );
	MSA_Check( AppInfo.hDC = GetDC(AppInfo.hWnd),	"ERROR : Creating GL Device Context",	"MSA_CreateOpenGL" );
	

	// Sort Pixel Format;
	GLuint	PixelFormat;
	static	PIXELFORMATDESCRIPTOR pfd =	{		// pfd Tells Windows How We Want Things To Be
		sizeof(PIXELFORMATDESCRIPTOR),			// size
		1,										// Version
		PFD_DRAW_TO_WINDOW |					// Format Must Support Window
		PFD_SUPPORT_OPENGL |					// Format Must Support OpenGL
		InitInfo.dwPixelFlags,					// Any options from user
		PFD_TYPE_RGBA,							// Format is RGBA
		InitInfo.iBitDepth,						// bits per pixel
		0, 0, 0, 0, 0, 0,						// Color Bits Ignored
		0,										// No Alpha Buffer
		0,										// Shift Bit Ignored
		0,										// No Accumulation Buffer
		0, 0, 0, 0,								// Accumulation Bits Ignored
		16,										// 16Bit Z-Buffer (Depth Buffer)
		0,										// No Stencil Buffer
		0,										// No Auxiliary Buffer
		PFD_MAIN_PLANE,							// Main Drawing Layer
		0,										// Reserved
		0, 0, 0									// Layer Masks Ignored
	};

	MSA_Check( PixelFormat = ChoosePixelFormat(AppInfo.hDC, &pfd),	"ERROR : Choosing PixelFormat",			"MSA_CreateOpenGL" );
	MSA_Check( SetPixelFormat(AppInfo.hDC, PixelFormat, &pfd),		"ERROR : Setting PixelFormat",			"MSA_CreateOpenGL" );
	MSA_Check( AppInfo.hRC = wglCreateContext(AppInfo.hDC),			"ERROR : Creating Rendering Context",	"MSA_CreateOpenGL");
	MSA_Check( wglMakeCurrent(AppInfo.hDC, AppInfo.hRC),			"ERROR : Activating Rendering Context", "MSA_CreateOpenGL");

	MSA_Check( MSA_WinUpdate(InitInfo.rcWindow.x, InitInfo.rcWindow.y),	"ERROR : Updating Window",		"MSA_CreateOpenGL");

	MSA_Return;
}




/*********************** Destroy OpenGL in Window ***************************************/
int MSA_DestroyOpenGL()
{
	if(AppInfo.bFullScreen) {
		ChangeDisplaySettings(NULL, 0);		// Switch to desktop
		ShowCursor(TRUE);					// Show mouse
	}


	// Release RC
	if(AppInfo.hRC) {					
		if(!wglMakeCurrent(NULL, NULL)) {
			DWORD dwError = GetLastError();
			Err("ERROR : Releasing DC and RC", "MSA_DestroyOpenGL");
		}

		if(!wglDeleteContext(AppInfo.hRC)) {
			Err("ERROR : Releasing Rendering Context", "MSA_DestroyOpenGL");
		}
		
		AppInfo.hRC = NULL;
	}
	

	// Release DC
	if(AppInfo.hDC && ! ReleaseDC(AppInfo.hWnd, AppInfo.hDC)) {
		Err("ERROR : Releasing Device Context", "MSA_DestroyOpenGL");
		AppInfo.hDC = NULL;
	}

	MSA_Return;
}


