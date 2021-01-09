#include "Koopas.h"
#include "QuestionBrick.h"
#include "Game.h"
#include "PlayScence.h"
#include "PSwitch.h"

CKoopas::CKoopas(int type)
{
	this->type = type;

	/*if (min != -1 && max != -1)
	{
		xMin = min;
		xMax = max;
	}*/

	SetState(KOOPAS_STATE_WALKING);

	SetBoundingBox();
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	DebugOut(L"[State] state: %d\n", state);
	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;
	// Simple fall down
	if (type == KOOPAS_GREEN_WING)
		vy += KOOPAS_WING_GRAVITY * dt;
	else
		vy += KOOPAS_GRAVITY * dt;

	// Giao nhau vs obj
	for (int i = 0; i < coObjects->size(); i++)
	{		
		if (this == coObjects->at(i))
			continue;
		if (AABBCheck(this, coObjects->at(i)))
		{
			OnIntersect(coObjects->at(i), coObjects);
		}
	}

	if (y > 432)
	{
		isDie = true;
		isDead = true;
	}

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (isDie)
		mario->canMultiScoreLand = false;

	if (state == KOOPAS_STATE_HOLD /*|| state == KOOPAS_STATE_SHAKE && mario->isHold*/)
	{
		vy = 0;

		if (mario != NULL)
		{
			if (mario->nx > 0)
			{
				if (mario->GetLevel() == MARIO_LEVEL_SMALL)
					SetPosition(mario->x + MARIO_SMALL_BBOX_WIDTH - 3, mario->y - MARIO_SMALL_BBOX_HEIGHT / 4);
				else
					SetPosition(mario->x + MARIO_BIG_BBOX_WIDTH - 3, mario->y + MARIO_BIG_BBOX_HEIGHT / 9);
			}
			else
			{
				if (mario->GetLevel() == MARIO_LEVEL_SMALL)
					SetPosition(mario->x + 3 - KOOPAS_BBOX_WIDTH, mario->y - MARIO_SMALL_BBOX_HEIGHT / 4);
				else
					SetPosition(mario->x + 3 - KOOPAS_BBOX_WIDTH, mario->y + MARIO_BIG_BBOX_HEIGHT / 9);
			}
		}
	}	

	if (/*state == KOOPAS_STATE_SHAKE*/isShaking == true)
	{
		if (state == KOOPAS_STATE_HIDE || state == KOOPAS_STATE_HOLD)
		{
			if (GetTickCount() - shake_start > 1000)
			{
				isShaking = false;
				SetState(KOOPAS_STATE_WALKING);
				y -= KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_HIDE;

				if (mario != NULL)
				{
					mario->canHold = false;
					mario->koopas = NULL;
					mario->isHold = false;
				}
			}
		}
	}
	else
	{
		if (state == KOOPAS_STATE_HIDE || state == KOOPAS_STATE_HOLD)
		{
			if (mario->isHold)
			{
				if (GetTickCount() - hide_start > 8000)
				{
					/*SetState(KOOPAS_STATE_SHAKE);*/
					isShaking = true;
					shake_start = GetTickCount();
					hide_start = 0;
				}
			}
			else
			{
				if (GetTickCount() - hide_start > 5000)
				{
					//SetState(KOOPAS_STATE_SHAKE);
					isShaking = true;
					shake_start = GetTickCount();
					hide_start = 0;
				}
			}
		}
	}

	if (state == KOOPAS_STATE_DIE)
	{
		/*x += dx;
		y += dy;*/

		return;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	/*if (state != KOOPAS_STATE_DIE)*/
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

		/*if (ny < 0 )
		{
			y += min_ty * dy + ny * 0.4f;
			vy = 0;

			if (state == KOOPAS_STATE_HIDE && yReverse == true)
			{
				vx = 0;
			}
		}*/

		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (ny < 0 && e->obj != NULL)
			{
				if (!dynamic_cast<CCoin*>(e->obj) && !dynamic_cast<CLeaf*>(e->obj) && !dynamic_cast<CMushRoom*>(e->obj) && !dynamic_cast<CGoomba*>(e->obj) && !dynamic_cast<CKoopas*>(e->obj))
				{
					if (!(dynamic_cast<CBreakableBrick*>(e->obj) && e->obj->GetState() == BREAKABLE_BRICK_STATE_COIN || dynamic_cast<CPSwitch*>(e->obj) && e->obj->GetState() == PSWITCH_STATE_HIT))
					{
						if (type == KOOPAS_GREEN_WING)
						{
							y = e->obj->y - (bottom - top);
							vy = -KOOPAS_WING_JUMP;
						}
						else
						{
							PreventMoveY(e->obj);

							if (state == KOOPAS_STATE_HIDE && yReverse == true)
							{
								vx = 0;
							}
						}
					}
				}
			}

			//if (dynamic_cast<CBox*>(e->obj) && type == KOOPAS_RED && state == KOOPAS_STATE_WALKING)
			//{
			//	/*if (e->nx != 0)
			//		x += dx;*/
			//	if (e->ny < 0)
			//	{
			//		if (left <= e->obj->left)
			//		{
			//			x = e->obj->left;
			//			vx = -vx;
			//		}
			//		if (right >= e->obj->right)
			//		{
			//			x = e->obj->right - (right - left);
			//			vx = -vx;
			//		}
			//	}
			//}
			if (dynamic_cast<CGoomba*>(e->obj))
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				/*if (state == KOOPAS_STATE_SPIN || state == KOOPAS_STATE_HOLD)
				{
					if (!goomba->isDie)
					{
						goomba->vx = -nx * 0.1f;
						goomba->vy = -0.2f;
						goomba->SetState(GOOMBA_STATE_DIE_REVERSE);
					}

					if (state == KOOPAS_STATE_HOLD)
					{
						vx = -nx * 0.07f;
						vy = -0.2f;
						SetState(KOOPAS_STATE_DIE);
					}
				}
				else*/ if (e->nx != 0 && !((state == KOOPAS_STATE_SPIN || state == KOOPAS_STATE_HOLD)))
				{
					PreventMoveX(nx, goomba);
					vx = -vx;
				}
			}
			else if (dynamic_cast<CParaGoomba*>(e->obj))
			{
				/*if (state == KOOPAS_STATE_SPIN || state == KOOPAS_STATE_HOLD)
				{
					CParaGoomba* para = dynamic_cast<CParaGoomba*>(e->obj);
					if (!para->isDie)
					{
						para->vx = -nx * 0.05f;
						para->vy = -0.1f;
						para->SetState(PARA_GOOMBA_STATE_DIE_REVERSE);
					}

					if (state == KOOPAS_STATE_HOLD)
					{
						vx = -nx * 0.07f;
						vy = -0.2f;
						SetState(KOOPAS_STATE_DIE);
					}
				}*/
			}
			else if (dynamic_cast<CKoopas*>(e->obj))
			{
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				/*if (state == KOOPAS_STATE_SPIN)
				{
					if (!koopas->isDie)
					{
						koopas->vx = -nx * 0.07f;
						koopas->vy = -0.2f;
						koopas->SetState(KOOPAS_STATE_DIE);
					}

					if (state == KOOPAS_STATE_HOLD)
					{
						vx = -nx * 0.07f;
						vy = -0.2f;
						SetState(KOOPAS_STATE_DIE);
					}
				}
				else*/ if (e->nx != 0 && !(state == KOOPAS_STATE_SPIN || state == KOOPAS_STATE_HOLD) && !(koopas->state == KOOPAS_STATE_SPIN || koopas->state == KOOPAS_STATE_HOLD))
				{
					PreventMoveX(nx, koopas);
					vx = -vx;
					e->obj->vx = -e->obj->vx;
				}
			}
			else if ((dynamic_cast<CBox*>(e->obj) || dynamic_cast<CGround*>(e->obj) || dynamic_cast<CWarpPipe*>(e->obj) || dynamic_cast<CBrick*>(e->obj)) && !(dynamic_cast<CBreakableBrick*>(e->obj) && e->obj->GetState() == BREAKABLE_BRICK_STATE_COIN))
			{
				if (e->nx != 0)
				{
					if (!dynamic_cast<CBox*>(e->obj) && !dynamic_cast<CLeaf*>(e->obj) && !dynamic_cast<CMushRoom*>(e->obj))
					{
						if (state == KOOPAS_STATE_SPIN)
						{
							if (dynamic_cast<CQuestionBrick*>(e->obj))
							{
								if (e->nx != 0)
								{
									CQuestionBrick* qBrick = dynamic_cast<CQuestionBrick*>(e->obj);
									if (qBrick->GetState() == QUESTION_BRICK_STATE_NORMAL)
									{
										switch (qBrick->type)
										{
										case BRICK_NORMAL:
											qBrick->SetState(QUESTION_BRICK_STATE_HIT_COIN);
											break;
										case BRICK_ITEM:
											switch (mario->GetLevel())
											{
											case MARIO_LEVEL_SMALL:
												if (x <= qBrick->x)
													qBrick->SetState(QUESTION_BRICK_STATE_HIT_MUSHROOM_LEFT);
												else
													qBrick->SetState(QUESTION_BRICK_STATE_HIT_MUSHROOM_RIGHT);
												break;
											case MARIO_LEVEL_BIG: case MARIO_LEVEL_RACCOON: case MARIO_LEVEL_FIRE:
												qBrick->SetState(QUESTION_BRICK_STATE_HIT_LEAF);
												break;
											default:
												break;
											}
											break;
										default:
											break;
										}
									}
								}
							}
							else if (dynamic_cast<CBreakableBrick*>(e->obj))
							{
								CBreakableBrick* bBrick = dynamic_cast<CBreakableBrick*>(e->obj);
								if (bBrick->GetState() == QUESTION_BRICK_STATE_NORMAL)
								{
									switch (bBrick->type)
									{
									case BREAKABLE_BRICK_TYPE_COIN:
										bBrick->SetState(BREAKABLE_BRICK_STATE_BREAK);
										break;
									case BREAKABLE_BRICK_TYPE_1UP_MUSHROOM:
										if (x <= bBrick->x)
											bBrick->SetState(BREAKABLE_BRICK_STATE_1UP_MUSHROOM_LEFT);
										else
											bBrick->SetState(BREAKABLE_BRICK_STATE_1UP_MUSHROOM_RIGHT);
										break;
										/*case BREAKABLE_BRICK_TYPE_P_SWITCH:
											bBrick->SetState(BREAKABLE_BRICK_STATE_P_SWITCH);
											break;*/
									default:
										break;
									}
								}
							}
						}

						if (!(dynamic_cast<CBreakableBrick*>(e->obj) && e->obj->GetState() == BREAKABLE_BRICK_STATE_COIN))
						{
							PreventMoveX(nx, e->obj);
							vx = -vx;
						}
					}
				}
				if (e->ny < 0 && type == KOOPAS_RED && state == KOOPAS_STATE_WALKING)
				{
					if (left < e->obj->left - KOOPAS_BBOX_WIDTH / 2)
					{
						x = e->obj->left - KOOPAS_BBOX_WIDTH / 2;
						vx = -vx;
					}
					if (right > e->obj->right + KOOPAS_BBOX_WIDTH / 2)
					{
						x = e->obj->right - KOOPAS_BBOX_WIDTH / 2;
						vx = -vx;
					}
				}
			}
			/*if (type == KOOPAS_RED)
			{
				if (state == KOOPAS_STATE_WALKING)
				{
					if (x <= xMin)
					{
						x = xMin;
						vx = -vx;
					}
					if (x >= xMax)
					{
						x = xMax;
						vx = -vx;
					}
				}
			}*/
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CKoopas::Render()
{
	int ani = KOOPAS_ANI_WALKING_LEFT;
	if (type == KOOPAS_GREEN_WING)
	{
		if (vx > 0)
			xReverse = true;
		else
			xReverse = false;
		ani = KOOPAS_ANI_WING;
	}
	else if (state == KOOPAS_STATE_HIDE || state == KOOPAS_STATE_HOLD || state == KOOPAS_STATE_DIE) {
		if (isShaking)
			ani = KOOPAS_ANI_SHAKE;
		else
			ani = KOOPAS_ANI_HIDE;
	}
	else if (state == KOOPAS_STATE_SPIN) {
		if (vx > 0) ani = KOOPAS_ANI_SPIN_RIGHT;
		else if (vx <= 0) ani = KOOPAS_ANI_SPIN_LEFT;
	}
	/*else if (state == KOOPAS_STATE_SHAKE) ani = KOOPAS_ANI_SHAKE;*/
	else if (vx > 0) ani = KOOPAS_ANI_WALKING_RIGHT;
	else if (vx <= 0) ani = KOOPAS_ANI_WALKING_LEFT;

	animation_set->at(ani)->Render(x, y, xReverse, yReverse);

	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		AddPoint(/*POINT_100*/);
		if (type == KOOPAS_GREEN_WING)
			type = KOOPAS_GREEN;
		yReverse = true;
		isDie = true;
		//die_start = GetTickCount();
		break;
	case KOOPAS_STATE_HIDE:
		if (type == KOOPAS_GREEN_WING)
			type = KOOPAS_GREEN;
		else
			hide_start = GetTickCount();
		break;
	case KOOPAS_STATE_SPIN:
		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer()->canMultiScoreLand = true;
		break;
	case KOOPAS_STATE_WALKING:
		yReverse = false;
		if (type == KOOPAS_GREEN_WING)
			vx = -KOOPAS_WALKING_WING_SPEED;
		else
			vx = -KOOPAS_WALKING_SPEED;
		break;
		/*case KOOPAS_STATE_HOLD:
			vx = -KOOPAS_WALKING_SPEED;
			break;*/
	case KOOPAS_STATE_SHAKE:
		shake_start = GetTickCount();
		break;
	}

}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_WALKING)
		bottom = y + KOOPAS_BBOX_HEIGHT;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT_HIDE;

	CGameObject::GetBoundingBox(left, top, right, bottom);
}

void CKoopas::OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs)
{
	if (!obj->isDie)
	{	
		if (dynamic_cast<CGoomba*>(obj))
		{
			CGoomba* goomba = dynamic_cast<CGoomba*>(obj);
			if (state == KOOPAS_STATE_SPIN || state == KOOPAS_STATE_HOLD)
			{
				if (!goomba->isDie)
				{
					goomba->vx = -nx * 0.1f;
					goomba->vy = -0.2f;
					goomba->SetState(GOOMBA_STATE_DIE_REVERSE);
				}

				if (state == KOOPAS_STATE_HOLD)
				{
					vx = -nx * 0.07f;
					vy = -0.2f;
					SetState(KOOPAS_STATE_DIE);
				}
			}
		}
		else if (dynamic_cast<CParaGoomba*>(obj))
		{
			if (state == KOOPAS_STATE_SPIN || state == KOOPAS_STATE_HOLD)
			{
				CParaGoomba* para = dynamic_cast<CParaGoomba*>(obj);
				if (!para->isDie)
				{
					para->vx = -nx * 0.05f;
					para->vy = -0.1f;
					para->SetState(PARA_GOOMBA_STATE_DIE_REVERSE);
				}

				if (state == KOOPAS_STATE_HOLD)
				{
					vx = -nx * 0.07f;
					vy = -0.2f;
					SetState(KOOPAS_STATE_DIE);
				}
			}
		}
		else if (dynamic_cast<CKoopas*>(obj))
		{
			CKoopas* koopas = dynamic_cast<CKoopas*>(obj);
			if (state == KOOPAS_STATE_SPIN || state == KOOPAS_STATE_HOLD)
			{
				if (!koopas->isDie)
				{
					koopas->vx = -nx * 0.07f;
					koopas->vy = -0.2f;
					koopas->SetState(KOOPAS_STATE_DIE);
				}

				if (state == KOOPAS_STATE_HOLD)
				{
					vx = -nx * 0.07f;
					vy = -0.2f;
					SetState(KOOPAS_STATE_DIE);
				}
			}
		}
		else if (dynamic_cast<CPiranha*>(obj))
		{
			CPiranha* piranha = dynamic_cast<CPiranha*>(obj);
			if (state == KOOPAS_STATE_SPIN || state == KOOPAS_STATE_HOLD)
			{
				piranha->SetState(PIRANHA_STATE_DIE);
			}

			if (state == KOOPAS_STATE_HOLD)
			{
				vx = -nx * 0.07f;
				vy = -0.2f;
				SetState(KOOPAS_STATE_DIE);
			}
		}
	}
}

void CKoopas::SetBoundingBox()
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_HIDE || state == KOOPAS_STATE_SPIN || state == KOOPAS_STATE_HOLD)
		bottom = y + KOOPAS_BBOX_HEIGHT_HIDE;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}