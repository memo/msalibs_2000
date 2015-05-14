#ifndef __MSA_rules_yeMSAUtils__
#define __MSA_rules_yeMSAUtils__

#include <memory.h>



#define Msg(msg,title)				MessageBox( NULL, msg, title, MB_OK | MB_ICONINFORMATION )
#define	Err(msg, title)				if(!AppInfo.dwError) { AppInfo.dwError++; MessageBox( NULL, msg, title, MB_OK | MB_ICONSTOP ); }
#define	Ask(msg)					(IDYES == MessageBox( NULL, msg, "Question from ye High 1", MB_YESNO | MB_ICONQUESTION))

//#define _CheckMsg(expr,msg)				if(!(BOOL)(expr)) { _Err(msg); abort(); }
//#define _CheckDef(expr)					_CheckMsg(expr, "Error");

#define	MSA_Check(expr, msg, title)	if(!(expr)) { Err(msg, title); AppInfo.bQuit = TRUE; return FALSE; }


#define MSA_Return					return ( ! ((BOOL)(AppInfo.dwError)) )


#define DoP(obj,method)				if(obj) obj->method;
#define DoO(obj,method)				if(obj.method) obj.method;
#define	MSA_CallBack(method)		if(AppInfo.CallBacks.method) AppInfo.CallBacks.method();

#define Del(p)						if(p) { delete p; p=0; }
#define Rel(p)						if(p) { p->Release(); p=0; }

#define ClearMe						memset(this,0,sizeof(*this))
#define ClearPointer(p)				memset(p,0,sizeof(*p))
#define ClearObject(o)				memset(&o,0,sizeof(o))

#define ChkFlag(val,flag)			((val) & (flag))
#define SetFlag(val,flag)			val |= flag
#define	ClrFlag(val,flag)			val &= (0xFFFF^flag)

	
#define FUCKER						__asm int 3




#endif // __MSA_rules_yeMSAUtils__
