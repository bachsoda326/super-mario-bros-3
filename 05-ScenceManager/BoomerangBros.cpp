#include "BoomerangBros.h"
#include "PlayScence.h"
#include "Boomerang.h"

CBoomerangBros::CBoomerangBros(float x)
{
	start_x = x;
	SetState(BOOMERANGBROS_STATE_WALKING);

	SetBoundingBox();
}

void CBoomerangBros::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CEnemy::Update(dt, coObjects);
	if (!isActive || isDead) return;

	// Simple fall down
	if (!CGame::GetInstance()->GetCurrentScene()->GetIsObjStop())
		vy += ENEMY_GRAVITY * dt;
		
	if (state != BOOMERANGBROS_STATE_DIE)
	{
		// Movement
		if (x <= start_x - BOOMERANGBROS_DISTANCE_X)
			vx = BOOMERANGBROS_WALKING_SPEED;
		else if (x >= start_x + BOOMERANGBROS_DISTANCE_X)
			vx = -BOOMERANGBROS_WALKING_SPEED;

		CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		float distanceX = (x + BOOMERANGBROS_BBOX_WIDTH / 2) - (mario->x + BOOMERANGBROS_BBOX_WIDTH / 2);
		// Follow Mario
		if (distanceX > 0)
		{
			xReverse = false;
			nx = -1;
		}
		else
		{
			xReverse = true;
			nx = 1;
		}

		// Throw 1
		if (wait_time != 0 && GetTickCount() - wait_time > BOOMERANGBROS_WAIT_TIME)
		{
			ThrowBoomerang();
			wait_time = 0;
			throw_time = GetTickCount();
		}
		// Throw 2
		if (throw_time != 0 && GetTickCount() - throw_time > BOOMERANGBROS_THROW_TIME)
		{
			ThrowBoomerang();
			throw_time = 0;
			wait_time = GetTickCount();
		}

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
			}

			// If isDie, do not col-X
			if (isDie)
				return;

			// Col-X
			if (e->nx != 0)
			{
				//// Ground
				//if (dynamic_cast<CGround*>(e->obj) || dynamic_cast<CWarpPipe*>(e->obj) || dynamic_cast<CBrick*>(e->obj))
				//{
				//	PreventMoveX(nx, e->obj);
				//	vx = -vx;
				//}
				//// Goomba
				//else if (dynamic_cast<CGoomba*>(e->obj))
				//{
				//	PreventMoveX(nx, e->obj);
				//	vx = -vx;
				//	e->obj->vx = -e->obj->vx;
				//}
				//// Koopas
				//else if (dynamic_cast<CKoopas*>(e->obj))
				//{
				//	CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				//	if (koopas->state != KOOPAS_STATE_SPIN && koopas->state != KOOPAS_STATE_HOLD)
				//	{
				//		PreventMoveX(nx, e->obj);
				//		vx = -vx;
				//		koopas->vx = -koopas->vx;
				//	}
				//}
			}
		}
		// Clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CBoomerangBros::Render()
{
	int ani = BOOMERANGBROS_ANI_WALKING;
	switch (state)
	{
	case BOOMERANGBROS_STATE_WALKING: case BOOMERANGBROS_STATE_DIE:
		ani = BOOMERANGBROS_ANI_WALKING;
		break;
	case BOOMERANGBROS_STATE_THROW:
		ani = BOOMERANGBROS_ANI_THROW;
		break;
	default:
		break;
	}

	animation_set->at(ani)->Render(x, y, xReverse, yReverse);
}

void CBoomerangBros::SetState(int state)
{
	if (state == ENEMY_STATE_RESPAWN)
	{
		CGameObject::SetState(BOOMERANGBROS_STATE_WALKING);
		vx = -((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer()->nx * BOOMERANGBROS_WALKING_SPEED;
		wait_time = GetTickCount();
		return;
	}

	CGameObject::SetState(state);

	switch (state)
	{
	case BOOMERANGBROS_STATE_DIE:
		AddPoint();
		yReverse = true;
		isDie = true;
		break;
	default:
		break;
	}
}

void CBoomerangBros::ThrowBoomerang()
{
	CBoomerang* boomerang = new CBoomerang(x, y, nx);	
	CGame::GetInstance()->GetCurrentScene()->GetFrontObjs()->push_back(boomerang);
}

void CBoomerangBros::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + BOOMERANGBROS_BBOX_WIDTH;
	b = t + BOOMERANGBROS_BBOX_HEIGHT;

	CGameObject::GetBoundingBox(l, t, r, b);
}

void CBoomerangBros::SetBoundingBox()
{
	left = x;
	top = y;
	right = left + BOOMERANGBROS_BBOX_WIDTH;
	bottom = top + BOOMERANGBROS_BBOX_HEIGHT;
}
