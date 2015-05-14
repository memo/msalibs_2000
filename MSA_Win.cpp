#include <windows.h>

#include "..\_MSA_Utils\MSA_Utils.h"
#include "MSA_Win.h"


MSA_WINAPP::MSA_WINAPP()
{
	CLEARME;
}


MSA_WINAPP::MSA_WINAPP(char* tszTitle, HINSTANCE thInst, WNDPROC tWndProc, HMENU thMenu)
{
	Init(tszTitle,thInst,tWndProc,thMenu);
}

	
MSA_WINAPP::~MSA_WINAPP()
{
	Destroy();
}


void MSA_WINAPP::Destroy()
{
	DEL(szTitle);
//	_del(szClassName);
}


void MSA_WINAPP::Init(char* tszTitle, HINSTANCE thInst, WNDPROC tWndProc, HMENU thMenu)
{
	CLEARME;
	
	hInst		= thInst;
	WndProc		= tWndProc;
	hMenu		= thMenu;
	dwStyle		= WS_OVERLAPPEDWINDOW;
	dwExStyle	= 0;
	szTitle		= new char[strlen(tszTitle)+1];	
	szClassName	= new char[7];	

	strcpy(szTitle,tszTitle); 
	strcpy(szClassName,"Defname");

	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInst;
	wc.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject (WHITE_BRUSH);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= szClassName;

	SetRect(&rcWindow,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT);
}


void MSA_WINAPP::SetXY(int x,int y,int lx,int ly)
{
	if(!hWnd) {
		SetRect(&rcWindow,x,y,lx,ly);
	}
}


void MSA_WINAPP::SetDim(int lx,int ly)
{
	if(!hWnd) {
		rcWindow.right	= lx+GetSystemMetrics(SM_CXSIZEFRAME)*2;;	
		rcWindow.bottom	= ly+GetSystemMetrics(SM_CYSIZEFRAME)*2+GetSystemMetrics(SM_CYMENU);
//		int cx = 640+GetSystemMetrics(SM_CXSIZEFRAME)*2;
//		int cy = 480+GetSystemMetrics(SM_CYSIZEFRAME)*2+GetSystemMetrics(SM_CYMENU);
	}
}

void MSA_WINAPP::PrepareBlack(bool bWindow)
{
	wc.hbrBackground	= (HBRUSH)GetStockObject (BLACK_BRUSH);
	if(!bWindow) {
		dwExStyle=WS_EX_TOPMOST;
		dwStyle=WS_POPUP;
	}
}

int MSA_WINAPP::CreateWin(int nCmdShow)
{
	RegisterClass(&wc);    
	hWnd = CreateWindowEx(dwExStyle, szClassName, szTitle, dwStyle,
			rcWindow.left, rcWindow.top, rcWindow.right, rcWindow.bottom, 
			NULL, hMenu, hInst, NULL );
    if (!hWnd) return 0;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    GetWindowRect(hWnd, &rcWindow);
	return 1;
}

void MSA_WINAPP::Active(bool tActive)
{        	
	bActive=tActive;
}

void MSA_WINAPP::Active(WPARAM wParam)
{        	
	bActive = (wParam == WA_ACTIVE) || (wParam == WA_CLICKACTIVE);
}

void MSA_WINAPP::TogglePause() 
{
	bPaused = !bPaused;
}

HDC MSA_WINAPP::GetDC()
{
	return ::GetDC(hWnd);
}
		

int MSA_WINAPP::ReleaseDC(HDC hDC)
{
	return ::ReleaseDC(hWnd,hDC);
}

		
int MSA_WINAPP::ErrorBox(LPCSTR szTitle, LPCSTR szError, bool bKill, int iRet)
{
    MessageBox(hWnd, szError, szTitle, MB_OK);
    if(bKill) DestroyWindow(hWnd);
    return iRet;
}

void MSA_WINAPP::SetCaption(LPCSTR szCaption) 
{
	SetWindowText(hWnd,szCaption);
}

void MSA_WINAPP::AfterWinMove()
{
	if(bActive) {
		GetWindowRect(hWnd, &rcWindow);
		GetClientRect(hWnd, &rcViewport);
		GetClientRect(hWnd, &rcScreen);
		ClientToScreen(hWnd, (POINT*)&rcScreen.left);
		ClientToScreen(hWnd, (POINT*)&rcScreen.right);
	}
}


/*
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg) {
        case WM_ACTIVATEAPP:
			MyApp->Active(wParam);
            return 0;
		case WM_MOUSEMOVE:
			break;
        case WM_KEYDOWN:
            switch (wParam) {
                case VK_ESCAPE:
				case VK_F12:
                    PostMessage(hWnd, WM_CLOSE, 0, 0);
					return 0;
            }
            break;
		case WM_MOVE:
			if(MyDDraw) MyDDraw->AfterWinMove();
			break;
        case WM_PAINT:
			if(MyDDraw) MyDDraw->AfterWinPaint();
            break;
         case WM_DESTROY:
            PostQuitMessage(0);
			return 0;
    }
	return DefWindowProc(hWnd,iMsg,wParam,lParam);
}
*/