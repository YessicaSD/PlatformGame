#include "UiItem.h"
#include "j1App.h"
#include "j1Render.h"


void UiItem::AddToPos(const iPoint & value)
{
	localPos += value;
	worldPos += value;
	hitBox.x = worldPos.x;
	hitBox.y = worldPos.y;
	/*if (this->childs.Count() > 0)
	{
		for (p2List_item<UiItem*>* thisItem = this->childs.start;thisItem; thisItem= thisItem->next)
		{
			thisItem->data->AddToPos(value);
		}
	}*/

}

void UiItem::DrawChildrens()
{
	if (this->childs.Count() > 0)
	{
		for (p2List_item<UiItem*>* thisItem = this->childs.start; thisItem; thisItem = thisItem->next)
		{
			if (thisItem->data->enable)
			{
				thisItem->data->Draw();
				if (thisItem->data->showHitBox)
				{
					
					App->render->DrawQuad(thisItem->data->hitBox, 255, 255, 255, 255, false, false);
				}
			}

		}
		for (p2List_item<UiItem*>* thisItem = this->childs.start; thisItem; thisItem = thisItem->next)
		{
			if (thisItem->data->enable && thisItem->data->childs.Count() > 0)
			{
				thisItem->data->DrawChildrens();
			}
		}
	}

}

bool UiItem::AddParent(UiItem * parent)
{
	assert(parent != nullptr);
	if (this->parent == nullptr)
	{
		this->parent = parent;
		parent->childs.add(this);
	}
	else
	{
		int pos = this->parent->childs.find(this);
		assert(pos != -1);

		p2List_item<UiItem*>* me = this->parent->childs.At(pos);
		this->parent->childs.del(me);
		this->parent = parent;
		parent->childs.add(this);
	}
	
	return false;
}


