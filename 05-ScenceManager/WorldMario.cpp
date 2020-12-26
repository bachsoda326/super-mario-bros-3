#include "WorldMario.h"

CWorldMario::CWorldMario()
{
	SetBoundingBox();
}

void CWorldMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;

	// Giao nhau vs các điểm map
	for (int i = 0; i < coObjects->size(); i++)
	{
		if (AABBCheck(this, coObjects->at(i)))
		{
			OnIntersect(coObjects->at(i), coObjects);
		}
	}
}

void CWorldMario::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CWorldMario::OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs)
{	
	// World Station
	if (dynamic_cast<CWorldStation*>(obj))
	{
		CWorldStation* station = dynamic_cast<CWorldStation*>(obj);
		if (curStation == NULL)
		{
			curStation = station;
		}
		if (curStation == station)
			return;

		if (vx > 0 && right - WORLD_MARIO_BBOX_WIDTH / 2 >= station->right - WORLD_STATION_BBOX_WIDTH / 2)
		{
			vx = 0;
			curStation = station;
		}
		if (vx < 0 && right - WORLD_MARIO_BBOX_WIDTH / 2 <= station->right - WORLD_STATION_BBOX_WIDTH / 2)
		{
			vx = 0;
			curStation = station;
		}
		if (vy > 0 && bottom - WORLD_MARIO_BBOX_HEIGHT / 2 >= station->bottom - WORLD_STATION_BBOX_HEIGHT)
		{
			vy = 0;
			curStation = station;
		}
		if (vy < 0 && bottom - WORLD_MARIO_BBOX_HEIGHT / 2 <= station->bottom - WORLD_STATION_BBOX_HEIGHT)
		{
			vy = 0;
			curStation = station;
		}
	}
}

void CWorldMario::GoLeft()
{
	if (curStation->canLeft && vx == 0 && vy == 0)
	{
		vx = -0.15f;
	}
}

void CWorldMario::GoTop()
{
	if (curStation->canTop && vx == 0 && vy == 0)
	{
		vy = -0.15f;
	}
}

void CWorldMario::GoRight()
{
	if (curStation->canRight && vx == 0 && vy == 0)
	{
		vx = 0.15f;
	}
}

void CWorldMario::GoBottom()
{
	if (curStation->canBottom && vx == 0 && vy == 0)
	{
		vy = 0.15f;
	}
}

void CWorldMario::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + WORLD_MARIO_BBOX_WIDTH;
	b = t + WORLD_MARIO_BBOX_HEIGHT;

	CGameObject::GetBoundingBox(l, t, r, b);
}

void CWorldMario::SetBoundingBox()
{
	left = x;
	top = y;
	right = left + WORLD_MARIO_BBOX_WIDTH;
	bottom = top + WORLD_MARIO_BBOX_HEIGHT;
}
