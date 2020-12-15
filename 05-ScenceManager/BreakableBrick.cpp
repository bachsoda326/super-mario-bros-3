#include "BreakableBrick.h"

CBreakableBrick::CBreakableBrick(int type)
{
	this->type = type;
}

void CBreakableBrick::Render()
{	
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CBreakableBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}
