#include "MushRoom.h"

void CMushRoom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CMushRoom::Render()
{
	animation_set->at(1)->Render(x, y);

	RenderBoundingBox();
}

void CMushRoom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + MUSHROOM_BBOX_WIDTH;
	b = y + MUSHROOM_BBOX_HEIGHT;
}
