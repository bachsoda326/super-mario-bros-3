#include <algorithm>
#include <assert.h>

#include "Mario.h"
#include "Utils.h"

#include "Game.h"


#include "Portal.h"
#include "Box.h"

CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_SMALL;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	DebugOut(L"[State] State: %d\n", state);
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += MARIO_GRAVITY * dt;

	if (vy <= -0.28f)
	{
		vy = -0.28f;
		canJumpHigher = false;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	/*if (GetTickCount() - run_start > MARIO_RUN_TIME)
	{
		if (run_start != 0)
			run_start -= 0.5;
		SetState(MARIO_STATE_RUN);
	}*/

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

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx);

		// block every object first!
		/*x += min_tx*dx + nx*0.4f;
		y += min_ty*dy + ny*0.4f;

		if (nx!=0) vx = 0;
		if (ny!=0) vy = 0;*/

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		/*if (nx != 0)
			vx = 0;*/
		if (ny != 0) vy = 0;

		if (ny < 0 && vy == 0)
		{
			if (canRepeatJump)
			{
				canJump = true;
				canJumpHigher = true;
			}
			else
			{
				canJump = false;
				canJumpHigher = false;
			}
		}


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBox*>(e->obj))
			{
				if (e->nx != 0)
					x += dx;

				/*if (e->ny == -1)
				{
					y += min_ty * dy + ny * 0.4f;
					vy = 0;
				}
				else if (e->ny == 1)
				{
					y += dy;
				}*/
			}
			/*else
			{
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;

				if (nx != 0) vx = 0;
				if (ny != 0) vy = 0;
			}*/
			else if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			} // if Koopas
			else if (dynamic_cast<CKoopas*>(e->obj)) // if e->obj is CKoopas 
			{
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);

				//if (koopas->GetState() == KOOPAS_STATE_HIDE)
				//{
				//	//koopas->SetPosition(x, y);
				//}
				// jump on top >> kill CKoopas, can kick or throw
				if (e->ny < 0)
				{
					if (koopas->GetState() == KOOPAS_STATE_WALKING)
					{
						koopas->SetState(KOOPAS_STATE_HIDE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
					else if (koopas->GetState() == KOOPAS_STATE_HIDE)
					{
						koopas->SetState(KOOPAS_STATE_SPIN);
						koopas->vx = this->nx * KOOPAS_SPIN_SPEED;
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (koopas->GetState() == KOOPAS_STATE_WALKING)
					{
						if (untouchable == 0)
						{
							if (koopas->GetState() == KOOPAS_STATE_WALKING)
							{
								if (level > MARIO_LEVEL_SMALL)
								{
									level = MARIO_LEVEL_SMALL;
									StartUntouchable();
								}
								else
									SetState(MARIO_STATE_DIE);
							}
						}
					}
					else if (koopas->GetState() == KOOPAS_STATE_HIDE)
					{
						if (state == MARIO_STATE_PREPARE_RUN || state == MARIO_STATE_RUN)
						{
							SetKoopas(koopas);
							SetState(MARIO_STATE_HOLD);
							koopas->SetState(KOOPAS_STATE_HOLD);
						}
						else
						{
							koopas->SetState(KOOPAS_STATE_SPIN);
							koopas->vx = this->nx * KOOPAS_SPIN_SPEED;
						}
					}
				}
			} // if Koopas
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
			else if (dynamic_cast<CMushRoom*>(e->obj))
			{
				//if (level == MARIO_LEVEL_SMALL)
					level = MARIO_LEVEL_BIG;
			}
			else if (dynamic_cast<CLeaf*>(e->obj))
			{
				//if (level == MARIO_LEVEL_BIG)
					level = MARIO_LEVEL_RACCOON;
			}
		}
		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CMario::Render()
{
	int ani = -1;
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_SMALL_DIE;
	else if (level == MARIO_LEVEL_BIG)
	{
		if (state == MARIO_STATE_IDLE)
		{
			if (nx > 0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
			else ani = MARIO_ANI_BIG_IDLE_LEFT;
		}		
		if (state == MARIO_STATE_WALKING)
		{
			if (nx > 0) ani = MARIO_ANI_BIG_WALKING_RIGHT;
			else ani = MARIO_ANI_BIG_WALKING_LEFT;
		}
		if (state == MARIO_STATE_JUMP_HIGH)
		{
			if (nx > 0) ani = MARIO_ANI_BIG_JUMP_RIGHT;
			else ani = MARIO_ANI_BIG_JUMP_LEFT;
		}
		if (state == MARIO_STATE_RUN)
		{
			if (nx > 0) ani = MARIO_ANI_BIG_RUN_RIGHT;
			else ani = MARIO_ANI_BIG_RUN_LEFT;
		}
		if (state == MARIO_STATE_PREPARE_RUN)
		{
			if (nx > 0) ani = MARIO_ANI_BIG_PREPARE_RUN_RIGHT;
			else ani = MARIO_ANI_BIG_PREPARE_RUN_LEFT;
		}
		if (state == MARIO_STATE_HOLD)
		{
			if (nx > 0) ani = MARIO_ANI_BIG_HOLD_RIGHT;
			else ani = MARIO_ANI_BIG_HOLD_LEFT;
		}
	}
	else if (level == MARIO_LEVEL_RACCOON)
	{
		if (state == MARIO_STATE_IDLE)
		{
			if (nx > 0) ani = MARIO_ANI_RACCOON_IDLE_RIGHT;
			else ani = MARIO_ANI_RACCOON_IDLE_LEFT;
		}
		if (state == MARIO_STATE_WALKING)
		{
			if (nx > 0) ani = MARIO_ANI_RACCOON_WALKING_RIGHT;
			else ani = MARIO_ANI_RACCOON_WALKING_LEFT;
		}
		if (state == MARIO_STATE_JUMP_HIGH)
		{
			if (nx > 0) ani = MARIO_ANI_RACCOON_JUMP_RIGHT;
			else ani = MARIO_ANI_RACCOON_JUMP_LEFT;
		}
		if (state == MARIO_STATE_RUN)
		{
			if (nx > 0) ani = MARIO_ANI_RACCOON_RUN_RIGHT;
			else ani = MARIO_ANI_RACCOON_RUN_LEFT;
		}
		if (state == MARIO_STATE_PREPARE_RUN)
		{
			if (nx > 0) ani = MARIO_ANI_RACCOON_WALKING_RIGHT;
			else ani = MARIO_ANI_RACCOON_WALKING_LEFT;
		}
		if (state == MARIO_STATE_HOLD)
		{
			if (nx > 0) ani = MARIO_ANI_RACCOON_HOLD_RIGHT;
			else ani = MARIO_ANI_RACCOON_HOLD_LEFT;
		}
	}
	else if (level == MARIO_LEVEL_FIRE)
	{
		if (state == MARIO_STATE_IDLE)
		{
			if (nx > 0) ani = MARIO_ANI_FIRE_IDLE_RIGHT;
			else ani = MARIO_ANI_FIRE_IDLE_LEFT;
		}
		if (state == MARIO_STATE_WALKING)
		{
			if (nx > 0) ani = MARIO_ANI_FIRE_WALKING_RIGHT;
			else ani = MARIO_ANI_FIRE_WALKING_LEFT;
		}
		if (state == MARIO_STATE_JUMP_HIGH)
		{
			if (nx > 0) ani = MARIO_ANI_FIRE_JUMP_RIGHT;
			else ani = MARIO_ANI_FIRE_JUMP_LEFT;
		}
		if (state == MARIO_STATE_RUN)
		{
			if (nx > 0) ani = MARIO_ANI_FIRE_RUN_RIGHT;
			else ani = MARIO_ANI_FIRE_RUN_LEFT;
		}
		if (state == MARIO_STATE_PREPARE_RUN)
		{
			if (nx > 0) ani = MARIO_ANI_FIRE_WALKING_RIGHT;
			else ani = MARIO_ANI_FIRE_WALKING_LEFT;
		}
		if (state == MARIO_STATE_HOLD)
		{
			if (nx > 0) ani = MARIO_ANI_FIRE_HOLD_RIGHT;
			else ani = MARIO_ANI_FIRE_HOLD_LEFT;
		}
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		if (state == MARIO_STATE_IDLE)
		{
			if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
			else ani = MARIO_ANI_SMALL_IDLE_LEFT;
		}
		if (state == MARIO_STATE_WALKING)
		{
			if (nx > 0) ani = MARIO_ANI_SMALL_WALKING_RIGHT;
			else ani = MARIO_ANI_SMALL_WALKING_LEFT;
		}
		if (state == MARIO_STATE_JUMP_HIGH)
		{
			if (nx > 0) ani = MARIO_ANI_SMALL_JUMP_RIGHT;
			else ani = MARIO_ANI_SMALL_JUMP_LEFT;
		}
		if (state == MARIO_STATE_RUN)
		{
			if (nx > 0) ani = MARIO_ANI_SMALL_RUN_RIGHT;
			else ani = MARIO_ANI_SMALL_RUN_LEFT;
		}
		if (state == MARIO_STATE_PREPARE_RUN)
		{
			if (nx > 0) ani = MARIO_ANI_SMALL_WALKING_RIGHT;
			else ani = MARIO_ANI_SMALL_WALKING_LEFT;
		}
		if (state == MARIO_STATE_HOLD)
		{
			if (nx > 0) ani = MARIO_ANI_SMALL_HOLD_RIGHT;
			else ani = MARIO_ANI_SMALL_HOLD_LEFT;
		}
	}

	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y, alpha);

	RenderBoundingBox();
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
		/*case MARIO_STATE_WALKING_RIGHT:
			vx = MARIO_WALKING_SPEED;
			nx = 1;
			break;
		case MARIO_STATE_WALKING_LEFT:
			vx = -MARIO_WALKING_SPEED;
			nx = -1;
			break;*/
	case MARIO_STATE_JUMP_HIGH:
		if (canJumpHigher)
			vy -= MARIO_JUMP_HIGH_SPEED_Y;
		break;
	case MARIO_STATE_JUMP_SHORT:
		vy = -MARIO_JUMP_SHORT_SPEED_Y;
		break;
	case MARIO_STATE_IDLE:
		vx = 0;
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	case MARIO_STATE_RUN:
		vx = 2 * nx * MARIO_WALKING_SPEED;
		break;
	}
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (level == MARIO_LEVEL_RACCOON && nx > 0)
		left = x;
		//left = x + 10;
	else 
		left = x;
	top = y;

	if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_RACCOON || level == MARIO_LEVEL_FIRE)
	{
		right = left + MARIO_BIG_BBOX_WIDTH;
		bottom = top + MARIO_BIG_BBOX_HEIGHT;
	}
	else
	{
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
	canJump = true;
	canJumpHigher = true;
}

