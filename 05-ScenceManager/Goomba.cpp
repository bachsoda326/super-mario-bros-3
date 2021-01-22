#include "Goomba.h"
#include "Point.h"
#include "ParaGoomba.h"
#include "Box.h"
#include "Ground.h"
#include "WarpPipe.h"
#include "Koopas.h"
#include "Brick.h"
#include "PlayScence.h"

CGoomba::CGoomba()
{
	vxSpawn = GOOMBA_WALKING_SPEED;
	SetState(GOOMBA_STATE_WALKING);

	SetBoundingBox();
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{	
	CEnemy::Update(dt, coObjects);
	if (!isActive || isDead) return;

	// Simple fall down
	if (!CGame::GetInstance()->GetCurrentScene()->GetIsObjStop())
		vy += ENEMY_GRAVITY * dt;

	// Jumped die
	if (state == GOOMBA_STATE_DIE && GetTickCount() - die_start > GOOMBA_DIE_TIME)
	{
		isDead = true;
		DeleteObjs(coObjects);
		return;
	}
	// Goomba wait to walk in TitleScene
	if (walk_start != 0 && GetTickCount() - walk_start > GOOMBA_WAIT_WALK_TIME)
	{
		vx = -GOOMBA_WALKING_SPEED;
		walk_start = 0;
	}

	if (state != GOOMBA_STATE_DIE_REVERSE)
	{
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;
		coEvents.clear();

		CalcPotentialCollisions(coObjects, coEvents);

		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// Col logic with other objs	
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			
			// Stand-Y ground
			if (ny < 0 && e->obj != NULL && !dynamic_cast<CEnemy*>(e->obj))
			{
				PreventMoveY(e->obj);
				if (isOnTitleScene && walk_start == 0)
					walk_start = GetTickCount();
			}

			// If isDie, do not col-X
			if (isDie) 
				return;

			// Col-X
			if (e->nx != 0)
			{
				// Ground
				if (dynamic_cast<CGround*>(e->obj) || dynamic_cast<CWarpPipe*>(e->obj) || dynamic_cast<CBrick*>(e->obj))
				{
					PreventMoveX(nx, e->obj);
					vx = -vx;
				}
				// Goomba
				else if (dynamic_cast<CGoomba*>(e->obj))
				{
					PreventMoveX(nx, e->obj);
					vx = -vx;
					e->obj->vx = -e->obj->vx;
				}
				// Koopas
				else if (dynamic_cast<CKoopas*>(e->obj))
				{
					CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
					if (koopas->state != KOOPAS_STATE_SPIN && koopas->state != KOOPAS_STATE_HOLD)
					{
						PreventMoveX(nx, e->obj);
						vx = -vx;
						koopas->vx = -koopas->vx;
					}
				}
			}
		}
		// Clean up collision events
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
	if (state == ENEMY_STATE_RESPAWN)
	{
		CGameObject::SetState(GOOMBA_STATE_WALKING);
		if (!isOnTitleScene)
			vx = -((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer()->nx * GOOMBA_WALKING_SPEED;
		return;
	}

	CGameObject::SetState(state);

	switch (state)
	{
	case GOOMBA_STATE_DIE:
		AddPoint();
		y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE;
		vx = 0;
		vy = 0;
		isDie = true;
		die_start = GetTickCount();
		break;
	case GOOMBA_STATE_DIE_REVERSE:
		AddPoint();
		yReverse = true;
		isDie = true;
		die_start = GetTickCount();
		break;
	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
		break;	
	}
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

void CGoomba::SetBoundingBox()
{
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;
	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else
		bottom = y + GOOMBA_BBOX_HEIGHT;
}
