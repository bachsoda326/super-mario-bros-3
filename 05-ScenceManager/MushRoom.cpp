#include "MushRoom.h"
#include "Ground.h"
#include "WarpPipe.h"
#include "Brick.h"
#include "Box.h"
#include "CloudTooth.h"
#include "PlayScence.h"

CMushRoom::CMushRoom()
{
	SetBoundingBox();
}

CMushRoom::CMushRoom(float x, float y, int nx, int type)
{	
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(MUSHROOM_ANI_SET);
	SetAnimationSet(ani_set);

	this->type = type;
	this->nx = nx;
	this->start_bottom = y;
	SetPosition(x, y);
	vy = -MUSHROOM_Y_SPEED;

	SetBoundingBox();
}

void CMushRoom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isDead) return;
	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;
	
	if (!isInitialized)
	{
		isDie = true;
		if (bottom > 0 && bottom <= start_bottom)
		{
			isInitialized = true;
			isDie = false;
			vx = nx * MUSHROOM_X_SPEED;
			vy = 0;
		}		
	}

	if (isInitialized)
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
			DeleteBehindObjs(coObjects);
			return;
		}
		// Fall
		if (y > CCamera::GetInstance()->GetTopMap() + CCamera::GetInstance()->GetHeightMap())
		{
			Dead();
			DeleteBehindObjs(coObjects);
			return;
		}
		// Simple fall down
		vy += MUSHROOM_GRAVITY * dt;		
		
		if (!isDie)
		{
			vector<LPCOLLISIONEVENT> coEvents;
			vector<LPCOLLISIONEVENT> coEventsResult;

			coEvents.clear();

			CalcPotentialCollisions(coObjects, coEvents);

			float min_tx, min_ty, nx = 0, ny;
			float rdx = 0;
			float rdy = 0;

			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

			// Col with objs
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				// Stand-Y
				if (ny < 0 && e->obj != NULL && (dynamic_cast<CGround*>(e->obj) || dynamic_cast<CWarpPipe*>(e->obj) || dynamic_cast<CBrick*>(e->obj) || dynamic_cast<CBox*>(e->obj) || dynamic_cast<CCloudTooth*>(e->obj)))
				{
					PreventMoveY(e->obj);
				}
				// Col-X
				if (e->nx != 0)
				{
					if (dynamic_cast<CGround*>(e->obj) || dynamic_cast<CWarpPipe*>(e->obj) || dynamic_cast<CBrick*>(e->obj))
					{
						PreventMoveX(nx, e->obj);
						vx = -vx;
					}
				}
			}

			// Clean up collision events
			for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		}
	}	
}

void CMushRoom::Render()
{
	int ani = -1;
	switch (type)
	{
	case MUSHROOM_TYPE_RED:
		ani = MUSHROOM_ANI_RED;
		break;
	case MUSHROOM_TYPE_1_UP:
		ani = MUSHROOM_ANI_1_UP;
		break;
	default:
		break;
	}
	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CMushRoom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + MUSHROOM_BBOX_WIDTH;
	b = y + MUSHROOM_BBOX_HEIGHT;

	CGameObject::GetBoundingBox(l, t, r, b);
}

void CMushRoom::SetBoundingBox()
{
	left = x;
	top = y;
	right = x + MUSHROOM_BBOX_WIDTH;
	bottom = y + MUSHROOM_BBOX_HEIGHT;
}
