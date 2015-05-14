#ifndef __MSA_rules_ye_WINAPP__
#define __MSA_rules_ye_WINAPP__

#include <windows.h>

#define _ErrorCheck(expr,str,App)	{ if (!expr) return App->ErrorBox("ERROR", str); }

class MSA_WINAPP 
{
	private:
		char		*szTitle;			// title of window
		char		*szClassName;		// name of window class

		HMENU		hMenu;				// handle to the menu

		WNDPROC		WndProc;			// window proc
		void		Destroy();	

	public:
		HWND		hWnd;				// handle to window
		HINSTANCE	hInst;				// handle to app instance

		RECT		rcWindow;			// RECT of whole window
		RECT		rcViewport;			// RECT of client window
		RECT		rcScreen;			// RECT in screen coor.

		bool		bActive;			// is window active
		bool		bPaused;			// is Application paused

		DWORD		dwStyle;			// window style flags
		DWORD		dwExStyle;			// window extended style flags
		
		WNDCLASS	wc;					// window class
		
		MSA_WINAPP();
		MSA_WINAPP(char* tszTitle, HINSTANCE thInst, WNDPROC tWndProc, HMENU thMenu=NULL);
		~MSA_WINAPP();

		void		Init(char* tszTitle, HINSTANCE thInst, WNDPROC tWndProc, HMENU thMenu=NULL);

		void		SetXY(int x,int y,int lx=CW_USEDEFAULT,int ly=CW_USEDEFAULT);
		void		SetDim(int lx=CW_USEDEFAULT,int ly=CW_USEDEFAULT);
		void		PrepareBlack(bool bWindow);

		int			CreateWin(int nCmdShow = SW_SHOWNORMAL);
		void		Active(bool tActive=true);
		void		Active(WPARAM wParam);
		void		TogglePause();

		HDC			GetDC(); 
		int			ReleaseDC(HDC hDC); 

		int			ErrorBox(LPCSTR szTitle, LPCSTR szError, bool bKill=false, int iRet=0);
		void		SetCaption(LPCSTR szCaption);

		void		AfterWinMove();

};

#endif //__MSA_rules_ye_WINAPP__
