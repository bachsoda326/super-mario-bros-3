#include "Point.h"

CPoint::CPoint(float x, float y, int type)
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(POINT_ANI_SET);
	SetAnimationSet(ani_set);

	this->type = type;
	SetPosition(x, y);
	appear_start = GetTickCount();
	vy = -0.04f;
}

void CPoint::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	y += dy;
	
	if (appear_start != 0 && GetTickCount() - appear_start > 500)
	{
		isDie = true;
		isDead = true;
		DeleteFrontObjs(coObjects);
		return;
	}
}

void CPoint::Render()
{
	int ani = -1;
	switch (type)
	{
	case POINT_100:
		ani = POINT_ANI_100;
		break;
	case POINT_200:
		ani = POINT_ANI_200;
		break;
	case POINT_400:
		ani = POINT_ANI_400;
		break;
	case POINT_800:
		ani = POINT_ANI_800;
		break;
	case POINT_1000:
		ani = POINT_ANI_1000;
		break;
	case POINT_2000:
		ani = POINT_ANI_2000;
		break;
	case POINT_4000:
		ani = POINT_ANI_4000;
		break;
	case POINT_8000:
		ani = POINT_ANI_8000;
		break;
	case POINT_1_UP:
		ani = POINT_ANI_1_UP;
		break;
	default:
		break;
	}

	animation_set->at(ani)->Render(x, y);
}
