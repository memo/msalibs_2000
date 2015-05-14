#ifndef __MSA_OPENGL__
#define __MSA_OPENGL__

#include "MSA_WinApp.h"



#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLu32.lib")
#pragma comment (lib, "GLaux.lib")



/******************************************* NODE *******************************************/

class MSA_NODE {
public:
	MSA_QUAT		quat;					// Quaternion for orientation
	MSA_MATRIX44	m44;					// Transformation Matrix

	MSA_FVECT3		v3LnrSpeed;				// Linear speed in units/sec
	MSA_FVECT4		v4AngSpeed;				// Angular speed in Radians/sec

	virtual void Init()									{ MSA_InitM44(m44); InitRot(); /*Pos = &(m44.t);*/ }
	virtual void InitRot()								{ MSA_InitQUAT(quat); UpdateMatrix(); }
	virtual void UpdateMatrix()							{ if(quat.bChanged)	{ MSA_Quat2M44(quat, m44); quat.bChanged = FALSE; } }

	virtual void SetPos(float X,float Y,float Z)		{ MSA_SetPosM44(m44, X, Y, Z); }

	virtual void Move(float X,float Y,float Z)			{ MSA_MoveM44f(m44, X, Y, Z); }
	virtual void Move(MSA_FVECT3 &v3,float fDist)		{ MSA_MoveM44(m44, v3, fDist); }

	virtual void Rotate(float X,float Y,float Z,float fRad, BOOL bSync)	{ MSA_RotQUAT_f(quat, X, Y, Z, fRad);	if(bSync) UpdateMatrix(); }
	virtual void Rotate(MSA_FVECT3 &v3,float fRad, BOOL bSync)			{ MSA_RotQUAT_v3(quat, v3, fRad);		if(bSync) UpdateMatrix(); }
	
	virtual void Render()	{ return; }
};






/********************** CAMERA - look in -Z, +X right, +Y up ********************************/

class MSA_CAMERA : public MSA_NODE {
public :	
	float			fFOV;
	float			fFront;
	float			fBack;
	BOOL			bProjChanged;

	MSA_MATRIX44	m44Inv;


	void Init()									{ MSA_NODE::Init();		MSA_InitM44(m44Inv); SetView(45, 1, 100); InitRot(); }
	void InitRot()								{ MSA_NODE::InitRot();	Pitch(-HALF_PI); }

	void InvMatrix()							{ if(m44.bChanged) { MSA_InvM44(m44, m44Inv); m44.bChanged = FALSE; } }

	void SetView(float fov, float f, float b)	{ fFOV=fov; fFront=f; fBack=b; bProjChanged=TRUE; }


	void Truck(float fDist)						{ Move( MSA_fV3(m44.z), -fDist); }
	void Strafe(float fDist)					{ Move( MSA_fV3(m44.x), fDist); }
	void Raise(float fDist)						{ Move(0, 0, fDist); }

	void Pitch(float fRad)						{ Rotate( MSA_fV3(m44.x),  fRad, TRUE); }
	void Turn(float fRad)						{ Rotate(0, 0, 1,fRad, TRUE); }
	void Roll(float fRad)						{ Rotate( MSA_fV3(m44.z),  fRad, TRUE); }
};


extern MSA_CAMERA Camera;



/********************************** TEXTURES & MATERIAL *******************************/

typedef struct {

} MSA_TEXTURE;



typedef struct {
//	DWORD			TextureID;
} MSA_MATERIAL;


typedef struct {
	float fu,fv;
} MSA_UV;



/********************************** GEOMETRY ************************************/


typedef struct {
	MSA_FVECT3		v3Pos;
	MSA_RGBA		Color;
} MSA_VERTEX;


typedef struct {
	DWORD			dwVertexID[3];
	MSA_FVECT3		v3Normal;

	MSA_UV			UV1[3];
	MSA_UV			UV2[3];
	MSA_MATERIAL*	Mat;
} MSA_FACE;


typedef struct _MSA_MESH{
	int				iNoVertex;
	int				iNoFace;

	MSA_VERTEX		*Vertex;
	MSA_FACE		*Face;

	MSA_MATERIAL	*Material;

	
	GLuint			DisplayList;

	void			(*Render)(_MSA_MESH *lpMesh);

} MSA_MESH;



inline static void RenderMeshWithDisplayList(MSA_MESH *lpMesh)
{
	glCallList(lpMesh->DisplayList);
}


inline static void RenderMeshWithOutDisplayList(MSA_MESH *lpMesh)
{
	glBegin(GL_TRIANGLES);

		for(int f=0; f<lpMesh->iNoFace; f++) {

			for(int i=0; i<3; i++) {
				int v = lpMesh->Face[f].dwVertexID[i];
				glColor4fv( MSA_pF(lpMesh->Vertex[v].Color) );
				glVertex3fv( MSA_pF(lpMesh->Vertex[v].v3Pos) );
				glTexCoord2fv( MSA_pF(lpMesh->Face[i].UV1) );
			}

			glNormal3fv( MSA_pF(lpMesh->Face[i].v3Normal) );
		}

	glEnd();
};





/********************************** OBJECTS ************************************/

class MSA_OBJECT : public MSA_NODE {
public:
	MSA_MESH		*lpMesh;
	MSA_MATERIAL	*lpMaterial;

	inline void		Render();
};


inline void MSA_OBJECT::Render()	// camera inverse must be on stack...
{
	glPushMatrix(); // save camera inverse

	glMultMatrixf( MSA_pF(m44) );

	lpMesh->Render(lpMesh);

	glPopMatrix();	// restore camera inverse
}




/********************************** SCENE ************************************/

#define	MAX_NODES		5000
#define	MAX_TEXTURES	5000



class MSA_SCENE {
public :
	MSA_CAMERA		Camera;
	
	MSA_NODE		Node[MAX_NODES];
	GLuint			glTexture[MAX_TEXTURES];

	DWORD			dwNoObjects;

	void			Update();
	void			Render();
};


//extern MSA_SCENE Scene;


/********************************************************************************************/
/*
AUX_RGBImageRec *LoadBMP(char* FName)
{
	FILE *f = NULL;
	if(!FName) return NULL;

	f = fopen(FName, "r");
	if(f) {
		fclose(f);
		return auxDIBImageLoad(FName);
	}
	
	return NULL;
}


int LoadGLTextures()
{
	int iStatus = FALSE;
	AUX_RGBImageRec *ti =0;
	GLuint *tt = uiTexture;
	
	if(ti = LoadBMP("data\\glass.bmp")) {
		iStatus = TRUE;
		glGenTextures(3, tt);		// Generate three texture names (ID's) and store array in uiTexture
		
		glBindTexture(GL_TEXTURE_2D, tt[2]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, ti->sizeX, ti->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, ti->data);
		
		glBindTexture(GL_TEXTURE_2D, tt[1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, ti->sizeX, ti->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, ti->data);

		glBindTexture(GL_TEXTURE_2D, tt[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, ti->sizeX, ti->sizeY, GL_RGB, GL_UNSIGNED_BYTE, ti->data);
	}

	if(ti) {
		if(ti->data) free(ti->data);
		free(ti);
	}

	return iStatus;
}




/********************************************************************************************/




int		MSA_InitOpenGL();									// Initialize OpenGL




/*************************** Rendering function ******************************************/
inline int MSA_Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	MSA_CallBack(lpRender);

	AppInfo.dwRenderCount++;

	MSA_Return;
}



/************************************************************************************/

#endif