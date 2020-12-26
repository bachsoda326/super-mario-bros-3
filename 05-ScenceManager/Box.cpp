#include "Box.h"

CBox::CBox(int r, int b)
{
	mRight = r;
	mBottom = b;

	SetBoundingBox();
}

void CBox::Render()
{
	RenderBoundingBox();
}

void CBox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = mRight;
	b = mBottom;

	CGameObject::GetBoundingBox(l, t, r, b);
}

void CBox::SetBoundingBox()
{
	left = x;
	top = y;
	right = mRight;
	bottom = mBottom;
}
