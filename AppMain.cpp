#include "MSAGL.h"
#include <stdarg.h>


#define GRIDNO		50
#define GRIDSIZE	5.0f

#define	POSSPEED	10.0f
#define	ROTSPEED	1.0f


enum {Mesh_Grid, Mesh_Room, Mesh_Cube, Mesh_Count};


//GLuint		uiTexture[MAXTEXTURE];
GLuint		dlMesh;
GLuint		dlFont2D = 0;
//GLuint		dlFont3D = 0;

int			iTexID			= 0;					// Change with F

BOOL		bLight			= FALSE;				// toggle with L
BOOL		bBlend			= FALSE;				// toggle with B
BOOL		bColor			= FALSE;				// toggle with C
BOOL		bDrawTwice		= FALSE;				// toggle with T
BOOL		bDepth			= FALSE;				// toggle with D
BOOL		bAdditive		= FALSE;				// toggle with A
BOOL		bWireframe		= FALSE;				// toggle with W
BOOL		bDislayList		= FALSE;				// toggle with O as in Object
BOOL		bInfo			= FALSE;				// toggle with I

float		fAlpha			= 0.5f;					// Change with +,-


MSA_FVECT4	f4LightPos		= { 0.0f, 0.0f, 2.0f, 1.0f };
MSA_RGBA	LightAmbient	= { 0.5f, 0.5f, 0.5f, 1.0f };
MSA_RGBA	LightDiffuse	= { 1.0f, 1.0f, 1.0f, 1.0f };



/***************************** Font Stuff **************************************/
void BuildFonts(GLvoid)								// Build Our Bitmap Font
{
	HFONT	hFont;										// Windows Font ID

	dlFont2D = glGenLists(96);							// Storage For 96 Characters
//	dlFont3D = glGenLists(96);

	hFont = CreateFont(-12, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DRAFT_QUALITY, FF_DONTCARE|DEFAULT_PITCH, "Arial");

	SelectObject(AppInfo.hDC, hFont);							// Selects The Font We Want

	wglUseFontBitmaps(AppInfo.hDC, 32, 96, dlFont2D);
//	wglUseFontOutlines(AppInfo.hDC,	32, 96, dlFont3D, 0.0f, 0.2f, WGL_FONT_POLYGONS, NULL);	
}

void KillFonts(GLvoid)
{
	glDeleteLists(dlFont2D, 96);
//	glDeleteLists(dlFont3D, 96);
}

inline void glPrint(GLuint dlFont, const char *fmt, ...)					// Custom GL "Print" Routine
{
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	    vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(dlFont-32);								// Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
}



/********************************************************************************************/
MSA_FVECT3	cc[] = {{1,0,0}, {0,1,0}, {0,0,1}, {1,0,1}, {0,1,1}, {1,1,0} };


void DrawGrid1()
{
	glBegin(GL_QUADS);
	for(float j = -GRIDNO; j<=GRIDNO; j++) {
		for(float i = -GRIDNO; i<=GRIDNO; i++) {
			glColor4f( 1, 0, 0, fAlpha);	glVertex3f( i * GRIDSIZE	,  j * GRIDSIZE,   0 );
			glColor4f( 0, 1, 0, fAlpha);	glVertex3f( i * GRIDSIZE	, (j+1) * GRIDSIZE, 0 );
			glColor4f( 0, 0, 1, fAlpha);	glVertex3f( (i+1) * GRIDSIZE, (j+1) * GRIDSIZE, 0 );
			glColor4f( 1, 0, 1, fAlpha);	glVertex3f( (i+1) * GRIDSIZE, j * GRIDSIZE,   0 );
		}
	}
	glEnd();
}



#define _FN(i, j)	_Sin( (j)* (i) * 1.5f * DEG2RAD + fTime * 5)* 1.0f

void DrawGrid2()
{
	float fTime = AppInfo.fTime;
	glBegin(GL_QUADS);
	for(float j = -GRIDNO; j<=GRIDNO; j++) {
		for(float i = -GRIDNO; i<=GRIDNO; i++) {
			glColor4f( 1, 0, 0, fAlpha);	glVertex3f( i * GRIDSIZE	, j * GRIDSIZE,		 _FN(i,   j)   * GRIDSIZE );
			glColor4f( 0, 1, 0, fAlpha);	glVertex3f( i * GRIDSIZE	, (j+1) * GRIDSIZE,  _FN(i,   j+1) * GRIDSIZE);
			glColor4f( 0, 0, 1, fAlpha);	glVertex3f( (i+1) * GRIDSIZE, (j+1) * GRIDSIZE,  _FN(i+1, j+1) * GRIDSIZE);
			glColor4f( 1, 0, 1, fAlpha);	glVertex3f( (i+1) * GRIDSIZE, j * GRIDSIZE,		 _FN(i+1, j)   * GRIDSIZE  );
		}
	}
	glEnd();
}


/********************************************************************************************/
int MyRender()
{

	if(Camera.bProjChanged) {
		Camera.bProjChanged = FALSE;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(Camera.fFOV, AppInfo.rcWindow.ar, Camera.fFront, Camera.fBack);

		glMatrixMode(GL_MODELVIEW);
	}
	
	Camera.InvMatrix();
	glLoadMatrixf (MSA_pF(Camera.m44Inv));
	
//	glBindTexture(GL_TEXTURE_2D, 0);
	if(bDislayList) glCallList(dlMesh + Mesh_Grid);
	else DrawGrid2();


	if(bInfo) {
//		SetBkColor(AppInfo.hDC, RGB(0, 0, 0));
//		SetTextColor(AppInfo.hDC, RGB(255, 255, 255));
//		TextOut(AppInfo.hDC, 0, 0, AppInfo.szDebugInfo, strlen(AppInfo.szDebugInfo));

		glLoadIdentity();
		glColor4f( 1, 1, 1, 1);
		glRasterPos3f(0, 0, -1);
//		glPrint(dlFont2D, "%u, %u", AppInfo.dwRenderFPS, AppInfo.dwUpdateFPS);
		glPrint(dlFont2D, AppInfo.szDebugInfo);
	}

	SwapBuffers(AppInfo.hDC);

	return TRUE;
}


/********************************************************************************************/
inline int DoKey(BYTE key, BOOL &bvar, GLenum glflag, int bch)
{
	if(MSA_IsButtonNew(key)) {
		bvar = !bvar;
		if(bvar) { if(glflag) glEnable(glflag); AppInfo.szFlags[bch] = key; }
		else { if(glflag) glDisable(glflag); AppInfo.szFlags[bch] = '_'; }
	}
	return bvar;
}


int MyUpdate()
{
	if(MSA_IsButtonNew(VK_RETURN))	Camera.InitRot();

	if(MSA_IsButtonNew(VK_SPACE))	{
		if(!Inputs.bLockMouse) {
			MSA_SetV2(Inputs.i2MouseScrOld, AppInfo.rcWindow.cx, AppInfo.rcWindow.cy);
			SetCursorPos(Inputs.i2MouseScrOld.x, Inputs.i2MouseScrOld.y);
			Inputs.bLockMouse = TRUE;
		} else Inputs.bLockMouse = FALSE;
	}


	int i=0;

		DoKey('C', bColor,		0,				i++);
		DoKey('D', bDepth,		GL_DEPTH_TEST,	i++);
		DoKey('L', bLight,		GL_LIGHTING,	i++);
		DoKey('T', bDrawTwice,	0,				i++);
		DoKey('B', bBlend,		GL_BLEND,		i++);	
		DoKey('O', bDislayList,	0,				i++);	
		DoKey('I', bInfo,		0,				i++);	
		
	if(	DoKey('A', bAdditive,	0,				i++)) glBlendFunc(GL_ONE, GL_ONE); else glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if(	DoKey('W', bWireframe,	0,				i++)) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	


	if(MSA_IsButtonOn(VK_UP))					Camera.Truck( POSSPEED * AppInfo.fSecs);
	if(MSA_IsButtonOn(VK_DOWN))				Camera.Truck(-POSSPEED * AppInfo.fSecs);

	if(MSA_IsButtonOn(VK_RIGHT))				Camera.Strafe( POSSPEED * AppInfo.fSecs);
	if(MSA_IsButtonOn(VK_LEFT))				Camera.Strafe(-POSSPEED * AppInfo.fSecs);


	if(MSA_IsButtonOn(VK_INSERT))				Camera.Raise( POSSPEED * AppInfo.fSecs);
	if(MSA_IsButtonOn(VK_DELETE))				Camera.Raise(-POSSPEED * AppInfo.fSecs);

	if(MSA_IsButtonOn(VK_ADD))					{ fAlpha *= 1.02f; if(fAlpha > 1) fAlpha = 1; }
	if(MSA_IsButtonOn(VK_SUBTRACT))			{ fAlpha /= 1.02f; if(fAlpha < 0) fAlpha = 0; }


	if(MSA_IsButtonOn(VK_HOME))				{ Camera.fFOV += 1.0f; Camera.bProjChanged = TRUE; }
	if(MSA_IsButtonOn(VK_END))					{ Camera.fFOV -= 1.0f; Camera.bProjChanged = TRUE; }


	if(Inputs.f2MouseSpeed.y)					Camera.Pitch(-Inputs.f2MouseSpeed.y * DEG2RAD);
	if(Inputs.f2MouseSpeed.x)					Camera.Turn(Inputs.f2MouseSpeed.x * DEG2RAD);

	if(MSA_IsButtonOn(MSA_MOUSE_LEFT))			Camera.Roll(-ROTSPEED * AppInfo.fSecs);
	if(MSA_IsButtonOn(MSA_MOUSE_RIGHT))		Camera.Roll( ROTSPEED * AppInfo.fSecs);


//	Camera.Raise(-2 * AppInfo.fSecs);
//	if(Camera.m44.t.z<0.5f) Camera.SetPos(Camera.m44.t.x, Camera.m44.t.y, 0.5f);

	return TRUE;
}



/********************************************************************************************/
int MyInitScene()
{
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);		// Allow perspective correction

	glDepthFunc(GL_LEQUAL);


	dlMesh = glGenLists(Mesh_Count);

	glLightfv(GL_LIGHT1, GL_AMBIENT,  MSA_pF(LightAmbient));
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  MSA_pF(LightDiffuse));
	glLightfv(GL_LIGHT1, GL_POSITION, MSA_pF(f4LightPos));

	glEnable(GL_LIGHT1);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	
	glLineWidth(1);

//	glAlphaFunc(GL_LEQUAL, 1);
//	glEnable(GL_ALPHA_TEST );

//	LoadGLTextures();


	glNewList(dlMesh + Mesh_Grid,GL_COMPILE);
		DrawGrid2();
	glEndList();

	BuildFonts();

	glFogi(GL_FOG_MODE, GL_LINEAR);						// GL_LINEAR, GL_EXP, and GL_EXP2
	glFogfv(GL_FOG_COLOR, MSA_pF(InitInfo.BackCol));	// Set Fog Color
	glFogf(GL_FOG_DENSITY, 1.0f);						// How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_NICEST );					// GL_NICEST GL_FASTEST 
	glFogf(GL_FOG_START, 1.0f);							// Fog Start Depth
	glFogf(GL_FOG_END, 150.0f);							// Fog End Depth
	glEnable(GL_FOG);	

	strcpy(AppInfo.szFlags, "___________________");

	

	Inputs.bLockMouse = TRUE;
	ShowCursor(FALSE);

	
	return TRUE;
}


int MyDestroy()
{
	KillFonts();
	return TRUE;
}


/********************* Will be filled by User ************************************************/
void MSA_Start()
{
	InitInfo.rcWindow.x		= 640;
	InitInfo.rcWindow.y		= 480;
	InitInfo.iBitDepth		= 16;
	InitInfo.bFullScreen	= TRUE;
	InitInfo.dwPixelFlags	= PFD_DOUBLEBUFFER;

	InitInfo.fUpdateFreq	= 100;

//	InitInfo.LoopMode		= MSA_OnceEveryFrame;
//	InitInfo.LoopMode		= MSA_MultiThreaded;
	InitInfo.LoopMode		= MSA_LoopEveryFrame;

	InitInfo.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	InitInfo.hCursor		= LoadCursor(NULL, IDC_ARROW);

	MSA_SetRGBA(InitInfo.BackCol, 0.0f, 0.0f, 0.2f, 1.0f)

	static char szTitle[] = "ye High 1";
	InitInfo.szTitle		= szTitle;


//	InitInfo.bFullScreen = ASK("Run in Fullscreen Mode?");

	Camera.Init();
	Camera.SetPos(0, 0, 1);
	Camera.SetView(80, .1f, 200);
	
	MSA_SetRender(MyRender);
	MSA_SetInitScene(MyInitScene);
	MSA_SetUpdate(MyUpdate);
	MSA_SetDestroy(MyDestroy);
}

