#ifndef ITEM_UI_H
#define ITEM_UI_H
#include "SDL/include/SDL.h"


#include "p2Point.h"
#include "p2List.h"

enum UI_STATES
{
	IDLE,
	HOVER,
	CLICK,
	MAX_STATES,
};

class UiItem
{
protected:
	iPoint localPos = {0,0};
	iPoint worldPos = {0,0};
	UiItem* parent = nullptr;
	p2List<UiItem*> childs;

public:
	bool showHitBox = false;
	bool enable = true;
	bool interactive = true;
	bool draggable = true;

	UI_STATES state = IDLE;
	SDL_Rect hitBox = {0,0,0,0};
	p2Point<int> pivot = {0,0};
	uint mouseButtonDown = 0;
	
	UiItem(const iPoint& pos, UiItem* const parent);
	UiItem(SDL_Rect hitBox, UiItem *const parent, p2Point<int> pivot = { 0,0 }); 
	
	iPoint UpdateScreenPos()
	{
		worldPos = localPos;
		for (UiItem*  thisParent = this->parent; thisParent; thisParent= thisParent->parent)
		{
			worldPos += thisParent->localPos;
		}
	
		return worldPos;
	}
	iPoint GetScreenPos()
	{
		return worldPos;
	}
	uint ReturnNumOfChilds() const 
	{
		return childs.Count();
	}
	void AddToPos(const iPoint& value);
	void DrawChildrens();
	

	virtual void Draw() {};
	virtual void OnClickDown(){}
	virtual void OnClickUp(){}
	
	~UiItem()
	{
		if (this->parent != nullptr)
		{
			UiItem* thisParent = this->parent;
			uint pos = thisParent->childs.find(this);
			thisParent->childs.del(thisParent->childs.At(pos));
			this->parent = nullptr;
		}
	}

};

#endif // !ITEM_UI_H

