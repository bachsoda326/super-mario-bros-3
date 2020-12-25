#include "WorldMario.h"

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

void CWorldMario::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + WORLD_MARIO_BBOX_WIDTH;
	b = t + WORLD_MARIO_BBOX_HEIGHT;

	CGameObject::GetBoundingBox(l, t, r, b);
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
		else if (curStation == station)
			return;

		if (vx != 0 && right / 2 == station->right / 2)
		{
			vx = 0;
			curStation = station;
		}
		if (vy != 0 && bottom / 2 == station->bottom / 2)
		{
			vy = 0;
			curStation = station;
		}
	}
}
