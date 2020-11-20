#include "Ground.h"

CGround::CGround(int r, int b)
{
	right = r;
	bottom = b;
}

void CGround::Render()
{
	RenderBoundingBox();
}

void CGround::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = right;
	b = bottom;

	CGameObject::GetBoundingBox(l, t, r, b);
}