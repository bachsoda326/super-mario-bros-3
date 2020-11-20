#include "Bullet.h"
#include "Box.h"
#include "Mario.h"

CBullet::CBullet()
{
	isDie = true;
	isDead = true;
}

void CBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BULLET_BBOX_WIDTH;
	bottom = y + BULLET_BBOX_HEIGHT;

	CGameObject::GetBoundingBox(left, top, right, bottom);
}

void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	
	if (y + BULLET_BBOX_HEIGHT > 432)
	{
		isDie = true;
		isDead = true;
	}

	if (state == BULLET_STATE_EXPLODE)
	{
		if (GetTickCount() - explode_start > 300)
			isDead = true;
		return;
	}

	// Simple fall down
	vy += BULLET_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		
		x += min_tx * dx + nx * 0.1f;
		y += min_ty * dy + ny * 0.1f;

		/*if (ny < 0)
		{
			y += min_ty * dy + ny * 0.4f;
			vy = -0.2f;
		}*/

		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (ny < 0 && e->obj != NULL)
			{
				vy = -0.2f;
				y = e->obj->y - (bottom - top);
			}

			if (dynamic_cast<CMario*>(e->obj))
			{
				x += dx;
				y += dy;
			}
			if (dynamic_cast<CBox*>(e->obj))
			{
				if (e->nx != 0)
					x += dx;
			}
			else if (nx != 0)
			{
				vx = 0;
				vy = 0;
				SetState(BULLET_STATE_EXPLODE);
			}
			if (dynamic_cast<CGoomba*>(e->obj))
			{
				vx = 0;
				vy = 0;
				SetState(BULLET_STATE_EXPLODE);

				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				if (!goomba->isDie)
				{
					goomba->vx = -nx * 0.05f;
					goomba->vy = -0.2f;
					goomba->SetState(GOOMBA_STATE_DIE_REVERSE);
				}				
			}
			else if (dynamic_cast<CKoopas*>(e->obj))
			{
				vx = 0;
				vy = 0;
				SetState(BULLET_STATE_EXPLODE);

				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				if (!koopas->isDie)
				{
					koopas->vx = -nx * 0.07f;
					koopas->vy = -0.2f;
					koopas->SetState(KOOPAS_STATE_DIE);
				}
			}
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CBullet::Render()
{
	int ani = -1;

	if (state == BULLET_STATE_FIRE) {
		if (vx > 0) ani = BULLET_ANI_FIRE_RIGHT;
		else if (vx <= 0) ani = BULLET_ANI_FIRE_LEFT;
	}
	if (state == BULLET_STATE_EXPLODE) {
		ani = BULLET_ANI_EXPLODE;
	}

	animation_set->at(ani)->Render(x, y, yReverse);

	RenderBoundingBox();
}

void CBullet::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{	
	case BULLET_STATE_FIRE:
		isDie = false;
		isDead = false;
		break;
	case BULLET_STATE_EXPLODE:
		vx = 0;
		vy = 0;
		isDie = true;
		explode_start = GetTickCount();
		break;		
	}
}
