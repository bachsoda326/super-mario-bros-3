#include "WorldStation.h"

CWorldStation::CWorldStation(int id, bool left, bool top, bool right, bool bottom)
{
	sceneId = id;
	canLeft = left;
	canTop = top;
	canRight = right;
	canBottom = bottom;

	SetBoundingBox();
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

void CWorldStation::SetBoundingBox()
{
	left = x;
	top = y;
	right = left + WORLD_STATION_BBOX_WIDTH;
	bottom = top + WORLD_STATION_BBOX_HEIGHT;
}
