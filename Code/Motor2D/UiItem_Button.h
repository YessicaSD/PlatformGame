#ifndef UIITEM_BUTTON_H
#define UIITEM_BUTTON_H
#include "UiItem.h"
#include "p2Point.h"
#include "j1Textures.h"

class UiItem_Button :public UiItem
{
protected:
	SDL_Rect frames[MAX_STATES];

public:
	SDL_Rect section = {0,0,0,0};

public:
	UiItem_Button(p2Point<int> pos, const SDL_Rect* section);
	

};

#endif