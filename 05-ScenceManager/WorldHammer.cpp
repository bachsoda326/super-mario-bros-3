#include "WorldHammer.h"

CWorldHammer::CWorldHammer(float left, float right)
{
	x_left = left;
	x_right = right;
	vx = 0.01f;
}

void CWorldHammer::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	
	x += dx;

	if (x < x_left)
	{
		x = x_left;
		vx = -vx;
		xReverse = false;
	}
	else if (x > x_right)
	{
		x = x_right;
		vx = -vx;
		xReverse = true;
	}
}

void CWorldHammer::Render()
{
	animation_set->at(0)->Render(x, y, xReverse);

	RenderBoundingBox();
}

