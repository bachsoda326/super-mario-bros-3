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
	vy = -LEAF_Y_SPEED;

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
			vx = -LEAF_X_SPEED;
			vy = LEAF_Y_SPEED;
			nx = -1;
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
		// Out of region
		if (x + right - left + width / 4 <= leftCamera || x - width / 4 >= rightCamera || y + top - bottom + height / 8 <= topCamera || y - height / 8 >= bottomCamera - CAMERA_DISTANCE_Y)
		{
			Dead();
			DeleteFrontObjs(coObjects);
			return;
		}
		// Fall
		if (y > CCamera::GetInstance()->GetTopMap() + CCamera::GetInstance()->GetHeightMap())
		{
			Dead();
			DeleteFrontObjs(coObjects);
			return;
		}
		// When stop move in left/right edge
		if (!isStop)
		{
			if (nx < 0 && x > start_x + LEAF_BBOX_WIDTH / 2 - LEAF_DISTANCE_X && x <= start_x + LEAF_BBOX_WIDTH / 2 - LEAF_DISTANCE_X_1 || nx > 0 && x >= start_x - LEAF_BBOX_WIDTH / 2 + LEAF_DISTANCE_X_1 && x < start_x - LEAF_BBOX_WIDTH / 2 + LEAF_DISTANCE_X)
				vy -= LEAF_Y_UP_SPEED;
			else if (x < start_x + LEAF_BBOX_WIDTH / 2 - LEAF_DISTANCE_X || x > start_x - LEAF_BBOX_WIDTH / 2 + LEAF_DISTANCE_X)
			{
				vx = 0;
				vy = 0;
				stopTime_start = GetTickCount();
				isStop = true;
				if (nx < 0)
					x = start_x + LEAF_BBOX_WIDTH / 2 - LEAF_DISTANCE_X_2;
				else
					x = start_x - LEAF_BBOX_WIDTH / 2 + LEAF_DISTANCE_X_2;
			}
		}
		// Move
		else
		{
			if (GetTickCount() - stopTime_start > LEAF_STOP_TIME)
			{
				stopTime_start = 0;
				nx = -nx;
				vx = nx * LEAF_X_SPEED;
				vy = LEAF_Y_SPEED;
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
