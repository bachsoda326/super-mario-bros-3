#include "Leaf.h"

CLeaf::CLeaf()
{
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
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;
	
	if (!isInitialized)
	{
		if (y <= start_top)
		{
			isInitialized = true;
			vx = -0.06f;
			nx = -1;
			vy = 0.05f;
		}		
	}
	else 
	{		
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
