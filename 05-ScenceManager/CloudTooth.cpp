#include "CloudTooth.h"

void CCloudTooth::Render()
{
	RenderBoundingBox();
}

void CCloudTooth::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + CLOUD_TOOTH_BBOX_WIDTH;
	b = y + CLOUD_TOOTH_BBOX_HEIGHT;

	CGameObject::GetBoundingBox(l, t, r, b);
}
