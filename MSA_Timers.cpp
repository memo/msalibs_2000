#include "MSA_WinApp.h"




/************************************************************************************/
void CALLBACK Timer_DisplayFPS(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2) 
{
	static DWORD dwLastTime = timeGetTime();
	DWORD dwNowTime = timeGetTime();

	sprintf(AppInfo.szDebugInfo,"%s     %u fps     %u    %u    %s", InitInfo.szTitle, AppInfo.dwRenderCount, AppInfo.dwUpdateCount, dwNowTime - dwLastTime, AppInfo.szFlags);

	AppInfo.dwRenderFPS = AppInfo.dwRenderCount;
	AppInfo.dwUpdateFPS = AppInfo.dwUpdateCount;
	
	SetWindowText(AppInfo.hWnd, AppInfo.szDebugInfo);
	dwLastTime = dwNowTime;

	AppInfo.dwRenderCount = 0;
	AppInfo.dwUpdateCount = 0;

}



/************************************************************************************/
void CALLBACK Timer_Update(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2)
{

	if(AppInfo.bActive && !AppInfo.bMessage && !AppInfo.bPause) {
		MSA_Update();
	}
}




/************************************************************************************/
int MSA_InitTimers()
{
	TIMECAPS tc;

	MSA_Check( timeGetDevCaps(&tc, sizeof(TIMECAPS))	== TIMERR_NOERROR, "ERROR : Getting Timer Information",		"MSA_InitTimers");
	
	AppInfo.iTimeRes = min(tc.wPeriodMin, tc.wPeriodMax);
	
	MSA_Check( timeBeginPeriod(AppInfo.iTimeRes)		== TIMERR_NOERROR, "ERROR : Initializing Timer Resolution",	"MSA_InitTimers") ;

	AppInfo.wFPSTimerID		= timeSetEvent( 1000, AppInfo.iTimeRes, Timer_DisplayFPS, 0, TIME_PERIODIC);
	MSA_Check( AppInfo.wFPSTimerID,	"ERROR : Creating FPS Timer Callback",		"MSA_InitTimers" );

	if((AppInfo.fUpdateFreq = InitInfo.fUpdateFreq) == 0) {
		InitInfo.LoopMode = MSA_OnceEveryFrame;
	} else {
		AppInfo.wUpdatePeriod = (WORD)(1000 / AppInfo.fUpdateFreq);
		AppInfo.fUpdateFreq = 1000.0f / AppInfo.wUpdatePeriod;
	}

	if(InitInfo.LoopMode == MSA_MultiThreaded) {
		AppInfo.wTimerID		= timeSetEvent( AppInfo.wUpdatePeriod, AppInfo.iTimeRes, Timer_Update, 0, TIME_PERIODIC);

		MSA_Check( AppInfo.wTimerID,		"ERROR : Creating Update Timer Callback",	"MSA_InitTimers" );
	}
	

	MSA_Return;
}




/************************************************************************************/
int MSA_KillTimers()
{
	MSA_Check( timeKillEvent(AppInfo.wFPSTimerID)	== TIMERR_NOERROR, "Deleting FPS Timer",			"MSA_KillTimers");
	
	if(AppInfo.wTimerID)
		MSA_Check( timeKillEvent(AppInfo.wTimerID)	== TIMERR_NOERROR, "Deleting Update Timer",			"MSA_KillTimers");
	
	MSA_Check( timeEndPeriod(AppInfo.iTimeRes)		== TIMERR_NOERROR, "Restoring Timer Resolution",	"MSA_KillTimers");

	MSA_Return;
}

