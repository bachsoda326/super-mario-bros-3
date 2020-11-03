#include "BreakableBrick.h"

void CBreakableBrick::Render()
{	
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CBreakableBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}
