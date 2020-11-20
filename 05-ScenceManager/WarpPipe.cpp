#include "WarpPipe.h"

CWarpPipe::CWarpPipe(int r, int b)
{
	right = r;
	bottom = b;
}

void CWarpPipe::Render()
{
	RenderBoundingBox();
}

void CWarpPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = right;
	b = bottom;

	CGameObject::GetBoundingBox(l, t, r, b);
}
