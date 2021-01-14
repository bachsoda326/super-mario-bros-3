#include "Leaf.h"
#include "PlayScence.h"

CLeaf::CLeaf()
{
	SetBoundingBox();
}

CLeaf::CLeaf(float x, float y)
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(LEAF_ANI_SET);
	SetAnimationSet(ani_set);

	start_top = y - LEAF_BBOX_TOP_HIGH;
	start_x = x;
	SetPosition(x, y - LEAF_BBOX_WIDTH);

	vy = -0.05f;

	SetBoundingBox();
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isDead) return;
	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;
	
	if (!isInitialized)
	{
		isDie = true;
		if (y <= start_top)
		{
			isInitialized = true;
			isDie = false;
			vx = -0.06f;
			nx = -1;
			vy = 0.05f;
		}		
	}
	else 
	{	
		float leftCamera = CCamera::GetInstance()->GetBound().left;
		float topCamera = CCamera::GetInstance()->GetBound().top;
		float rightCamera = CCamera::GetInstance()->GetBound().right;
		float bottomCamera = CCamera::GetInstance()->GetBound().bottom;
		float width = CGame::GetInstance()->GetScreenWidth();
		float height = CGame::GetInstance()->GetScreenHeight();

		if (x + right - left + width / 4 <= leftCamera || x - width / 4 >= rightCamera || y + top - bottom + height / 8 <= topCamera || y - height / 8 >= bottomCamera - 24)
		{
			Dead();
			DeleteFrontObjs(coObjects);
			return;
		}

		switch (((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetMap()->GetId())
		{
		case MAP_1_1:
			if (y > HEIGHT_MAP_1_1)
			{
				Dead();
				DeleteFrontObjs(coObjects);
				return;
			}
			break;
		default:
			break;
		}

		if (!isStop)
		{
			if (nx < 0 && x > start_x + LEAF_BBOX_WIDTH / 2 - 30 && x <= start_x + LEAF_BBOX_WIDTH / 2 - 20 || nx > 0 && x >= start_x - LEAF_BBOX_WIDTH / 2 + 20 && x < start_x - LEAF_BBOX_WIDTH / 2 + 30)
				vy -= 0.008f;
			else if (x < start_x + LEAF_BBOX_WIDTH / 2 - 30 || x > start_x - LEAF_BBOX_WIDTH / 2 + 30)
			{
				vx = 0;
				vy = 0;
				stopTime_start = GetTickCount();
				isStop = true;
				if (nx < 0)
					x = start_x + LEAF_BBOX_WIDTH / 2 - 26;
				else
					x = start_x - LEAF_BBOX_WIDTH / 2 + 26;
			}
		}
		else
		{
			if (GetTickCount() - stopTime_start > 50)
			{
				stopTime_start = 0;				
				
				nx = -nx;
				vx = nx * 0.06f;
				vy = 0.05f;
				xReverse = !xReverse;
				isStop = false;
			}
		}		
	}
}

void CLeaf::Render()
{
	animation_set->at(2)->Render(x, y, xReverse);

	RenderBoundingBox();
}

void CLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + LEAF_BBOX_WIDTH;
	b = y + LEAF_BBOX_HEIGHT;

	CGameObject::GetBoundingBox(l, t, r, b);
}

void CLeaf::SetBoundingBox()
{
	left = x;
	top = y;
	right = x + LEAF_BBOX_WIDTH;
	bottom = y + LEAF_BBOX_HEIGHT;
}
