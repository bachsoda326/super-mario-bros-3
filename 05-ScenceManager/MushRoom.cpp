#include "MushRoom.h"
#include "Ground.h"
#include "WarpPipe.h"
#include "Brick.h"
#include "Box.h"

CMushRoom::CMushRoom()
{
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

	vy = -0.02f;
}

void CMushRoom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;	

	// not set to bottom yet
	if (!isInitialized && bottom > 0 && bottom <= start_bottom)
	{
		isInitialized = true;
		vy = 0;
		vx = nx * 0.05f;
	}

	if (isInitialized)
	{
		// Simple fall down
		vy += MUSHROOM_GRAVITY * dt;

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		CalcPotentialCollisions(coObjects, coEvents);

		// No collision occured, proceed normally
		if (coEvents.size() == 0)
		{
			/*x += dx;
			y += dy;*/
		}
		else
		{
			float min_tx, min_ty, nx = 0, ny;
			float rdx = 0;
			float rdy = 0;

			// TODO: This is a very ugly designed function!!!!
			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

			//
			// Collision logic with other objects
			//
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];

				if (ny < 0 && e->obj != NULL && (dynamic_cast<CGround*>(e->obj) || dynamic_cast<CWarpPipe*>(e->obj) || dynamic_cast<CBrick*>(e->obj) || dynamic_cast<CBox*>(e->obj)))
				{
					PreventMoveY(e->obj);
				}

				if (e->nx != 0)
				{
					if (dynamic_cast<CGround*>(e->obj) || dynamic_cast<CWarpPipe*>(e->obj) || dynamic_cast<CBrick*>(e->obj))
					{
						PreventMoveX(nx, e->obj);
						vx = -vx;
					}
				}
			}

			// clean up collision events
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
	animation_set->at(MUSHROOM_ANI_1_UP)->Render(x, y);

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
