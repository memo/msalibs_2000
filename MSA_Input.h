#ifndef __MSA_rules_ye_Inputs__
#define __MSA_rules_ye_Inputs__


#include "MSA_Math.h"



#define		MAXBUTTONS						256 + 3			// 256 Keyboard keys and 3 mouse buttons


#define		BUTTONS							Inputs.Buttons


#define		MSA_MOUSE_LEFT					256
#define		MSA_MOUSE_RIGHT					257
#define		MSA_MOUSE_MIDDLE				258

#define		MSA_BUTTON_ON					1				// Bit to set if button is on
#define		MSA_BUTTON_READ					2				// Bit to set if button was read after being pressed


#define		MSA_SetButton(iBNo, state)		if(state) BUTTONS[iBNo] |= MSA_BUTTON_ON; else BUTTONS[iBNo] = 0;
#define		MSA_IsButtonNew(iBNo)			(BUTTONS[iBNo] && !(BUTTONS[iBNo] & MSA_BUTTON_READ)) ? (BUTTONS[iBNo] |=MSA_BUTTON_READ, TRUE) : FALSE
#define		MSA_IsButtonOn(iBNo)			BUTTONS[iBNo]



typedef struct __INPUTS {
	BYTE		Buttons[MAXBUTTONS];		// Key bits + 3 mouse buttons
	MSA_IVECT2	i2MouseScr;					// Mouse cursor position on screen
	MSA_IVECT2	i2MouseScrOld;				// Last Mouse Position on screen
	MSA_IVECT2	i2Mouse;					// Mouse cursor position in client coors
	MSA_FVECT2	f2MouseSpeed;				// Mouse Speed (in pixels/sec independant of resolution and time)
	BOOL		bLockMouse;
} INPUTS;


#endif	//__MSA_rules_ye_Inputs__
