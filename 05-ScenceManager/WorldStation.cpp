#include "WorldStation.h"

CWorldStation::CWorldStation(bool left, bool top, bool right, bool bottom)
{
	canLeft = left;
	canTop = top;
	canRight = right;
	canBottom = bottom;
}

void CWorldStation::Render()
{
	RenderBoundingBox();
}

void CWorldStation::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + WORLD_STATION_BBOX_WIDTH;
	b = t + WORLD_STATION_BBOX_HEIGHT;

	CGameObject::GetBoundingBox(l, t, r, b);
}
