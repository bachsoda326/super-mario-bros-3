#include "ParaGoomba.h"
#include "Ground.h"
#include "WarpPipe.h"
#include "Brick.h"
#include "Goomba.h"
#include "Mario.h"
#include "PlayScence.h"

CParaGoomba::CParaGoomba()
{
	level = PARA_GOOMBA_LEVEL_WING;
	vxSpawn = PARA_GOOMBA_WALKING_SPEED;
	SetState(PARA_GOOMBA_STATE_WALKING);
	vx = -PARA_GOOMBA_WALKING_SPEED;

	SetBoundingBox();
}

void CParaGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CEnemy::Update(dt, coObjects);
	if (!isActive || isDead) return;

	// Simple fall down
	if (state != PARA_GOOMBA_STATE_DIE_REVERSE)
		vy += PARA_GOOMBA_GRAVITY * dt;
	else
		vy += ENEMY_GRAVITY * dt;

	if (state == PARA_GOOMBA_STATE_DIE && GetTickCount() - die_start > GOOMBA_DIE_TIME)
	{
		isDead = true;
		DeleteObjs(coObjects);
		return;
	}
	
	// Movement
	if (level == PARA_GOOMBA_LEVEL_WING)
	{
		// Follow Mario
		CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		float distanceX = (x + PARA_GOOMBA_WING_BBOX_WIDTH / 2) - (mario->x + MARIO_BIG_BBOX_WIDTH / 2);

		if (abs(distanceX) >= PARA_GOOMBA_DISTANCE_MIN_X && abs(distanceX) <= PARA_GOOMBA_DISTANCE_MAX_X)
		{
			if (distanceX < 0)
				vx = PARA_GOOMBA_WALKING_SPEED;
			else
				vx = -PARA_GOOMBA_WALKING_SPEED;
		}

		// Wing movement
		if (state == PARA_GOOMBA_STATE_WALKING && GetTickCount() - wing_walk_start > PARA_GOOMBA_WALK_TIME)
		{
			SetState(PARA_GOOMBA_STATE_PREPARE_JUMP);
		}
		if (state == PARA_GOOMBA_STATE_PREPARE_JUMP && GetTickCount() - wing_pre_jump_start > PARA_GOOMBA_PREPARE_JUMP_TIME)
		{
			SetState(PARA_GOOMBA_STATE_JUMP);
		}
	}

	if (state != PARA_GOOMBA_STATE_DIE_REVERSE)
	{
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		CalcPotentialCollisions(coObjects, coEvents);

		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// Collision logic with other objects		
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (ny < 0 && e->obj != NULL && !dynamic_cast<CMario*>(e->obj) && !dynamic_cast<CEnemy*>(e->obj))
			{
				switch (state)
				{
				case PARA_GOOMBA_STATE_WALKING: case PARA_GOOMBA_STATE_LOOSE_WING: case PARA_GOOMBA_STATE_DIE:
					vy = 0;
					break;
				case PARA_GOOMBA_STATE_PREPARE_JUMP:
					vy = -PARA_GOOMBA_PREPARE_JUMP_SPEED;
					break;
				case PARA_GOOMBA_STATE_JUMP:
					SetState(PARA_GOOMBA_STATE_WALKING);
					break;
				default:
					break;
				}

				y = e->obj->y - (bottom - top);
			}		

			// If isDie, do not coll
			if (isDie)
				return;

			if (e->nx != 0)
			{
				if (dynamic_cast<CGround*>(e->obj) || dynamic_cast<CWarpPipe*>(e->obj) || dynamic_cast<CBrick*>(e->obj))
				{
					PreventMoveX(nx, e->obj);
					vx = -vx;
				}
			}
			else if (e->ny > 0 && dynamic_cast<CBrick*>(e->obj))
			{
				vy = 0;
			}
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CParaGoomba::Render()
{
	int ani = PARA_GOOMBA_ANI_WALKING;
	
	if (level == PARA_GOOMBA_LEVEL_WING)
	{
		switch (state)
		{
		case PARA_GOOMBA_STATE_WALKING:
			ani = PARA_GOOMBA_ANI_WING_WALKING;
			break;
		case PARA_GOOMBA_STATE_PREPARE_JUMP:
			ani = PARA_GOOMBA_ANI_WING_PREPARE_JUMP;
			break;
		case PARA_GOOMBA_STATE_JUMP:
			ani = PARA_GOOMBA_ANI_WING_JUMP;
			break;		
		}	
	}
	else if (level == PARA_GOOMBA_LEVEL_NOR)
	{
		switch (state)
		{
		case PARA_GOOMBA_STATE_WALKING: case PARA_GOOMBA_STATE_DIE_REVERSE:
			ani = PARA_GOOMBA_ANI_WALKING;
			break;
		case PARA_GOOMBA_STATE_DIE:
			ani = PARA_GOOMBA_ANI_DIE;
			break;		
		}
	}

	animation_set->at(ani)->Render(x, y, xReverse, yReverse);
	RenderBoundingBox();
}

void CParaGoomba::SetState(int state)
{
	if (state == ENEMY_STATE_RESPAWN)
	{
		CGameObject::SetState(PARA_GOOMBA_STATE_WALKING);

		vx = -((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer()->nx * PARA_GOOMBA_WALKING_SPEED;
		vy = 0;
		if (level == PARA_GOOMBA_LEVEL_WING)
			wing_walk_start = GetTickCount();
		return;
	}

	CGameObject::SetState(state);

	switch (state)
	{	
	case PARA_GOOMBA_STATE_WALKING:
		vy = 0;
		if (level == PARA_GOOMBA_LEVEL_WING)
			wing_walk_start = GetTickCount();
		break;
	case PARA_GOOMBA_STATE_PREPARE_JUMP:
		wing_pre_jump_start = GetTickCount();
		vy = -PARA_GOOMBA_PREPARE_JUMP_SPEED;
		break;
	case PARA_GOOMBA_STATE_JUMP:
		vy = -PARA_GOOMBA_JUMP_SPEED;
		break;
	case PARA_GOOMBA_STATE_DIE:
		AddPoint();
		y += PARA_GOOMBA_BBOX_HEIGHT - PARA_GOOMBA_BBOX_HEIGHT_DIE;
		vx = 0;
		vy = 0;
		isDie = true;
		die_start = GetTickCount();
		break;
	case PARA_GOOMBA_STATE_DIE_REVERSE:
		AddPoint();
		level = PARA_GOOMBA_LEVEL_NOR;
		yReverse = true;
		isDie = true;
		die_start = GetTickCount();
		break;	
	case PARA_GOOMBA_STATE_LOOSE_WING:
		AddPoint();
		level = PARA_GOOMBA_LEVEL_NOR;
		state = PARA_GOOMBA_STATE_WALKING;
		vy = 0;
		break;
	}
}

void CParaGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (level == PARA_GOOMBA_LEVEL_WING)
	{
		right = x + PARA_GOOMBA_WING_BBOX_WIDTH;
		bottom = y + PARA_GOOMBA_WING_BBOX_HEIGHT;
	}
	else if (level == PARA_GOOMBA_LEVEL_NOR)
	{
		right = x + PARA_GOOMBA_BBOX_WIDTH;
		if (state == PARA_GOOMBA_STATE_DIE)
			bottom = y + PARA_GOOMBA_BBOX_HEIGHT_DIE;
		else
			bottom = y + PARA_GOOMBA_BBOX_HEIGHT;
	}

	CGameObject::GetBoundingBox(left, top, right, bottom);
}

void CParaGoomba::SetBoundingBox()
{
	left = x;
	top = y;
	if (level == PARA_GOOMBA_LEVEL_WING)
	{
		right = x + PARA_GOOMBA_WING_BBOX_WIDTH;
		bottom = y + PARA_GOOMBA_WING_BBOX_HEIGHT;
	}
	else if (level == PARA_GOOMBA_LEVEL_NOR)
	{
		right = x + PARA_GOOMBA_BBOX_WIDTH;
		if (state == PARA_GOOMBA_STATE_DIE)
			bottom = y + PARA_GOOMBA_BBOX_HEIGHT_DIE;
		else
			bottom = y + PARA_GOOMBA_BBOX_HEIGHT;
	}
}
