#include "Piranha.h"

CPiranha::CPiranha(int type, float x, float y)
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(PIRANHA_ANI_SET);
	SetAnimationSet(ani_set);

	this->type = type;
	start_bottom = y;
	switch (type)
	{
	case PIRANHA_NOR:
		start_top = start_bottom - PIRANHA_NOR_BBOX_HEIGHT;
		break;
	case PIRANHA_FIRE:
		start_top = start_bottom - PIRANHA_FIRE_BBOX_HEIGHT;
		break;
	case PIRANHA_FIRE_RED:
		start_top = start_bottom - PIRANHA_FIRE_RED_BBOX_HEIGHT;
		break;
	default:
		break;
	}
	SetPosition(x, y);

	vy = -0.025f;
}

void CPiranha::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	CGameObject::Update(dt, coObjs);
		
	y += dy;

	if (y < start_top)
	{
		y = start_top;
		wait_up_start = GetTickCount();
		vy = 0;
	}
	if (y > start_bottom)
	{
		y = start_bottom;
		wait_down_start = GetTickCount();
		vy = 0;
	}

	if (wait_up_start != 0 && GetTickCount() - wait_up_start > 1500)
	{
		wait_up_start = 0;
		vy = 0.025f;
	}
	if (wait_down_start != 0 && GetTickCount() - wait_down_start > 2500)
	{
		wait_down_start = 0;
		vy = -0.025f;
	}
}

void CPiranha::Render()
{
	int ani = 0;
	switch (type)
	{
	case PIRANHA_NOR:
		ani = 2;
		break;
	case PIRANHA_FIRE:
		ani = 3;
		break;
	case PIRANHA_FIRE_RED:
		ani = 0;
		break;
	default:
		break;
	}
	animation_set->at(ani)->Render(x, y, xReverse, yReverse);

	RenderBoundingBox();
}

void CPiranha::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + PIRANHA_BBOX_WIDTH;
	switch (type)
	{
	case PIRANHA_NOR:
		b = t + PIRANHA_NOR_BBOX_HEIGHT;
		break;
	case PIRANHA_FIRE:
		b = t + PIRANHA_FIRE_BBOX_HEIGHT;
		break;
	case PIRANHA_FIRE_RED:
		b = t + PIRANHA_FIRE_RED_BBOX_HEIGHT;
		break;
	default:
		break;
	}

	CGameObject::GetBoundingBox(l, t, r, b);
}


