#ifndef __MSA_rules_ye_WORLD2D__
#define __MSA_rules_ye_WORLD2D__


#include "..\_MSA_DDraw\MSA_DDraw.h"
#include "MSA_sprite.h"

#define MAX_SPRITE_COUNT 512

class MSA_WORLD2D 							// SPRITE LINKED LIST MANAGMENT
{
	private:
		friend class MSA_SPRITE;

		MSA_DDRAW	*TheDDraw;

		MSA_SPRITE	*spr;
		short		SpriteCount;

		void		Init();
		void		Destroy();

	public:
		MSA_WORLD2D();	
		MSA_WORLD2D(MSA_DDRAW *tTheDDraw);

		~MSA_WORLD2D();
	
		void		WriteText(int x,int y,LPCTSTR lpString);
		void		PutPixel(int x,int y,int c);

		MSA_SPRITE* AddSprite(LPCSTR szBitmap);
		MSA_SPRITE* AddSprite(MSA_SPRITE* ss=NULL);
		void		DeleteAllSprites();

//		int RemoveSPRITE(short i);
		
		MSA_SPRITE* Sprite(short i);
		void		Clear(int Color=0);
		void		Update();
		void		Render();
};


#endif	// __MSA_rules_ye_MSA_WORLD2D__
