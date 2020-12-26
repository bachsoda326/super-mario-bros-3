#include "Brick.h"

CBrick::CBrick()
{
	SetBoundingBox();
}

void CBrick::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;

	CGameObject::GetBoundingBox(l, t, r, b);
}

void CBrick::SetBoundingBox()
{
	left = x;
	top = y;
	right = x + BRICK_BBOX_WIDTH;
	bottom = BRICK_BBOX_HEIGHT;
}