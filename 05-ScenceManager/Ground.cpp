#include "Ground.h"

CGround::CGround(int r, int b)
{
	mRight = r;
	mBottom = b;

	SetBoundingBox();
}

void CGround::Render()
{	
	RenderBoundingBox();
}

void CGround::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = mRight;
	b = mBottom;

	CGameObject::GetBoundingBox(l, t, r, b);
}

void CGround::SetBoundingBox()
{
	left = x;
	top = y;
	right = mRight;
	bottom = mBottom;
}