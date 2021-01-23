#include "Boomerang.h"
#include "PlayScence.h"

CBoomerang::CBoomerang(float x, float y, int direction)
{
	start_x = x;
	start_y = y;
	nx = direction;
	start_time = GetTickCount();

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(BOOMERANG_ANI_SET);
	SetAnimationSet(ani_set);

	SetBoundingBox();
}

void CBoomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	if (isDead) return;		
	
	// Movement like cicle
	x = start_x + nx * BOOMERANG_X_DISTANCE + nx * BOOMERANG_X_CENTER * cos(PI * (GetTickCount() - start_time) / BOOMERANG_DIVISION + PI);
	y = start_y + nx * BOOMERANG_Y_CENTER * sin(-nx * PI * (GetTickCount() - start_time) / BOOMERANG_DIVISION);

	if (!canCatch && GetTickCount() - start_time > BOOMERANG_CAN_CATCH_TIME)
		canCatch = true;

	float leftCamera = CCamera::GetInstance()->GetBound().left;
	float topCamera = CCamera::GetInstance()->GetBound().top;
	float rightCamera = CCamera::GetInstance()->GetBound().right;
	float bottomCamera = CCamera::GetInstance()->GetBound().bottom;
	float width = CGame::GetInstance()->GetScreenWidth();
	float height = CGame::GetInstance()->GetScreenHeight();
	// Out of region
	if (x + right - left + width / 4 <= leftCamera || x - width / 4 >= rightCamera || y + top - bottom + height / 8 <= topCamera || y - height / 8 >= bottomCamera - CAMERA_DISTANCE_Y)
	{
		Dead();
		DeleteFrontObjs(coObjects);			
		return;
	}

	// Intersect with Boomerang Bros
	if (canCatch)
	{
		for (int i = 0; i < coObjects->size(); i++)
		{
			if (!coObjects->at(i)->isDie && dynamic_cast<CBoomerangBros*>(coObjects->at(i)))
			{
				if (AABBCheck(this, coObjects->at(i)))
				{
					OnIntersect(coObjects->at(i), coObjects);
				}
			}
		}
	}
}

void CBoomerang::Render()
{
	animation_set->at(0)->Render(x, y);

	RenderBoundingBox();
}

void CBoomerang::OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs)
{
	Dead();
	DeleteFrontObjs(coObjs);
}

void CBoomerang::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + BOOMERANG_BBOX_SIZE;
	b = t + BOOMERANG_BBOX_SIZE;

	CGameObject::GetBoundingBox(l, t, r, b);
}

void CBoomerang::SetBoundingBox()
{
	left = x;
	top = y;
	right = left + BOOMERANG_BBOX_SIZE;
	bottom = top + BOOMERANG_BBOX_SIZE;
}
