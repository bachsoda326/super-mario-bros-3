#include "Box.h"

CBox::CBox(int r, int b)
{
	right = r;
	bottom = b;
}

void CBox::Render()
{
	RenderBoundingBox();
}

void CBox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = right;
	b = bottom;

	CGameObject::GetBoundingBox(l, t, r, b);
}
