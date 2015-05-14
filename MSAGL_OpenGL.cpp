/**************************************************

	All functions dealing with standard OpenGL stuff


****************************************************/


#include "MSAGL.h"

MSA_CAMERA Camera;



/************************ Init OpenGL Parameters *********************************/
int MSA_InitOpenGL()
{
	glEnable(GL_TEXTURE_2D);		// Enable Texture mapping
	glShadeModel(GL_SMOOTH);		// Set shading mode
	glClearColor(InitInfo.BackCol.r, InitInfo.BackCol.g, InitInfo.BackCol.b, InitInfo.BackCol.a);		// Set background color
	glClearDepth(1);				// Sort Depth buffer
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	
	MSA_Return;
}
