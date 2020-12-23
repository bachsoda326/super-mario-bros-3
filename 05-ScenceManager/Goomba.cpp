#include "Goomba.h"
CGoomba::CGoomba()
{
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;

	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else
		bottom = y + GOOMBA_BBOX_HEIGHT;

	CGameObject::GetBoundingBox(left, top, right, bottom);
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
		
	x += dx;
	y += dy;
	// Simple fall down
	vy += GOOMBA_GRAVITY * dt;

	if (y + GOOMBA_BBOX_HEIGHT > 432)
	{
		isDie = true;
		isDead = true;
	}

	if (state == GOOMBA_STATE_DIE && GetTickCount() - die_start > 300)
		isDead = true;

	if (state == GOOMBA_STATE_DIE_REVERSE)
	{
		/*x += dx;
		y += dy;*/

		if (GetTickCount() - die_start > 1000)
			isDead = true;
		return;
	}

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
		/*x += dx;
		y += dy;*/
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		
		/*x += min_tx * dx + nx * 0.1f;
		y += min_ty * dy + ny * 0.1f;*/


		/*if (ny < 0)
		{
			y += min_ty * dy + ny * 0.4f;
			vy = 0;
		}*/

		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (ny < 0 && e->obj != NULL)
			{
				PreventMoveY(e->obj);
			}

			/*if (dynamic_cast<CBox*>(e->obj))
			{
				if (e->nx != 0)
					x += dx;
			}*/
			/*if (dynamic_cast<CGround*>(e->obj) || dynamic_cast<CWarpPipe*>(e->obj) || dynamic_cast<CBrick*>(e->obj))*/

			if (e->nx != 0)
			{
				if (dynamic_cast<CGround*>(e->obj) || dynamic_cast<CWarpPipe*>(e->obj) || dynamic_cast<CBrick*>(e->obj) || dynamic_cast<CGoomba*>(e->obj))
				{
					PreventMoveX(nx, e->obj);
					vx = -vx;
				}
				else if (dynamic_cast<CKoopas*>(e->obj))
				{
					CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
					if (koopas->state != KOOPAS_STATE_SPIN && koopas->state != KOOPAS_STATE_HOLD)
					{
						PreventMoveX(nx, e->obj);
						vx = -vx;						
					}
				}
				/*if (dynamic_cast<CGoomba*>(e->obj) || dynamic_cast<CMario*>(e->obj))
				{
					PreventMoveX(nx, e->obj);
					vx = -vx;
				}*/
				/*else if (dynamic_cast<CBox*>(e->obj))
					x += dx;*/
			}
			/*else
				x += dx;*/
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CGoomba::Render()
{
	int ani = GOOMBA_ANI_WALKING;
	if (state == GOOMBA_STATE_DIE) {
		ani = GOOMBA_ANI_DIE;
	}
	else if (state == GOOMBA_STATE_DIE_REVERSE) {
		ani = GOOMBA_ANI_WALKING;
	}

	animation_set->at(ani)->Render(x, y, xReverse, yReverse);

	RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE;
		vx = 0;
		vy = 0;
		isDie = true;
		die_start = GetTickCount();
		break;
	case GOOMBA_STATE_DIE_REVERSE:
		yReverse = true;
		isDie = true;
		die_start = GetTickCount();
		break;
	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
		break;
	}
}
