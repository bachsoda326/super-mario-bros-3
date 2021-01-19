#include "Koopas.h"
#include "QuestionBrick.h"
#include "Game.h"
#include "PlayScence.h"
#include "PSwitch.h"

CKoopas::CKoopas(int type)
{
	this->type = type;
	if (this->type == KOOPAS_GREEN_WING)
		vxSpawn = KOOPAS_WALKING_WING_SPEED;
	else
		vxSpawn = KOOPAS_WALKING_SPEED;
	SetState(KOOPAS_STATE_WALKING);

	SetBoundingBox();
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CEnemy::Update(dt, coObjects);
	if (!isActive || isDead) return;
	UpdateDirection();

	// Simple fall down
	if (type != KOOPAS_RED_WING)
	{
		if (type == KOOPAS_GREEN_WING)
			vy += KOOPAS_WING_GRAVITY * dt;
		else
			vy += ENEMY_GRAVITY * dt;
	}

	// Intersect with objs
	for (int i = 0; i < coObjects->size(); i++)
	{
		if (!coObjects->at(i)->isDie)
		{
			if (this == coObjects->at(i))
				continue;
			if (AABBCheck(this, coObjects->at(i)))
			{
				OnIntersect(coObjects->at(i), coObjects);
			}
		}
	}

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (isDie)
		mario->canMultiScoreLand = false;

	// Koopas when being holded by Mario
	if (state == KOOPAS_STATE_HOLD)
	{
		vy = 0;

		if (mario != NULL)
		{
			if (mario->nx > 0)
			{
				if (mario->GetLevel() == MARIO_LEVEL_SMALL)
					SetPosition(mario->x + MARIO_SMALL_BBOX_WIDTH - KOOPAS_HOLD_DISTANCE_X, mario->y - MARIO_SMALL_BBOX_HEIGHT / KOOPAS_HOLD_DISTANCE_Y_SMALL);
				else
					SetPosition(mario->x + MARIO_BIG_BBOX_WIDTH - KOOPAS_HOLD_DISTANCE_X, mario->y + MARIO_BIG_BBOX_HEIGHT / KOOPAS_HOLD_DISTANCE_Y_BIG);
			}
			else
			{
				if (mario->GetLevel() == MARIO_LEVEL_SMALL)
					SetPosition(mario->x + KOOPAS_HOLD_DISTANCE_X - KOOPAS_BBOX_WIDTH, mario->y - MARIO_SMALL_BBOX_HEIGHT / KOOPAS_HOLD_DISTANCE_Y_SMALL);
				else
					SetPosition(mario->x + KOOPAS_HOLD_DISTANCE_X - KOOPAS_BBOX_WIDTH, mario->y + MARIO_BIG_BBOX_HEIGHT / KOOPAS_HOLD_DISTANCE_Y_BIG);
			}
		}
	}
	// Wake up after a period
	if (/*state == KOOPAS_STATE_SHAKE*/isShaking == true)
	{
		if (state == KOOPAS_STATE_HIDE || state == KOOPAS_STATE_HOLD)
		{
			if (GetTickCount() - shake_start > KOOPAS_SHAKE_TIME)
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
				if (GetTickCount() - hide_start > KOOPAS_HIDE_HOLD_TIME)
				{
					/*SetState(KOOPAS_STATE_SHAKE);*/
					isShaking = true;
					shake_start = GetTickCount();
					hide_start = 0;
				}
			}
			else
			{
				if (GetTickCount() - hide_start > KOOPAS_HIDE_TIME)
				{
					//SetState(KOOPAS_STATE_SHAKE);
					isShaking = true;
					shake_start = GetTickCount();
					hide_start = 0;
				}
			}
		}
	}

	if (type == KOOPAS_RED_WING)
	{
		if (y <= yMin)
			vy = 0.05f;
		else if (y >= yMax)
			vy = -0.05f;
	}

	if (state != KOOPAS_STATE_DIE)
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

			if (ny < 0 && e->obj != NULL)
			{
				if (!dynamic_cast<CCoin*>(e->obj) && !dynamic_cast<CLeaf*>(e->obj) && !dynamic_cast<CMushRoom*>(e->obj) && !dynamic_cast<CEnemy*>(e->obj))
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
			// Goomba
			if (dynamic_cast<CGoomba*>(e->obj))
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				if (e->nx != 0 && !((state == KOOPAS_STATE_SPIN || state == KOOPAS_STATE_HOLD)))
				{
					PreventMoveX(nx, goomba);
					vx = -vx;
					goomba->vx = -goomba->vx;
				}
			}
			// Koopas		
			else if (dynamic_cast<CKoopas*>(e->obj))
			{
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				if (e->nx != 0 && !(state == KOOPAS_STATE_SPIN || state == KOOPAS_STATE_HOLD) && !(koopas->state == KOOPAS_STATE_SPIN || koopas->state == KOOPAS_STATE_HOLD))
				{
					PreventMoveX(nx, koopas);
					vx = -vx;
					e->obj->vx = -e->obj->vx;
				}
			}
			// Others
			else if ((dynamic_cast<CBox*>(e->obj) || dynamic_cast<CGround*>(e->obj) || dynamic_cast<CWarpPipe*>(e->obj) || dynamic_cast<CBrick*>(e->obj)) && !(dynamic_cast<CBreakableBrick*>(e->obj) && e->obj->GetState() == BREAKABLE_BRICK_STATE_COIN))
			{
				if (e->nx != 0)
				{
					if (!dynamic_cast<CBox*>(e->obj) && !dynamic_cast<CLeaf*>(e->obj) && !dynamic_cast<CMushRoom*>(e->obj))
					{
						if (state == KOOPAS_STATE_SPIN)
						{
							// Question brick
							if (dynamic_cast<CQuestionBrick*>(e->obj))
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
							// Breakable brick
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
									case BREAKABLE_BRICK_TYPE_P_SWITCH:
										bBrick->SetState(BREAKABLE_BRICK_STATE_P_SWITCH);
										break;
									case BREAKABLE_BRICK_TYPE_ITEM:
										switch (mario->GetLevel())
										{
										case MARIO_LEVEL_SMALL:
											if (x <= bBrick->x)
												bBrick->SetState(BREAKABLE_BRICK_STATE_HIT_MUSHROOM_LEFT);
											else
												bBrick->SetState(BREAKABLE_BRICK_STATE_HIT_MUSHROOM_RIGHT);
											break;
										case MARIO_LEVEL_BIG: case MARIO_LEVEL_RACCOON: case MARIO_LEVEL_FIRE:
											bBrick->SetState(BREAKABLE_BRICK_STATE_HIT_LEAF);
											break;
										default:
											break;
										}
										break;
									case BREAKABLE_BRICK_TYPE_MULTI_COIN:
										if (mario->GetLevel() > MARIO_LEVEL_SMALL)
										{
											bBrick->SetState(BREAKABLE_BRICK_STATE_HIT_MULTI_COIN);
											bBrick->vy = -0.1f;
										}
										break;
									default:
										break;
									}
								}
							}
						}
						// Col-X
						if (!(dynamic_cast<CBreakableBrick*>(e->obj) && e->obj->GetState() == BREAKABLE_BRICK_STATE_COIN))
						{
							PreventMoveX(nx, e->obj);
							vx = -vx;
						}
					}
				}
				if (e->ny < 0 && state == KOOPAS_STATE_WALKING)
				{
					// Case Mario hit Brick to hit Koopas
					if (dynamic_cast<CBrick*>(e->obj))
					{
						CBrick* brick = dynamic_cast<CBrick*>(e->obj);
						if (brick->isUp)
						{
							yReverse = true;
							vx = mario->nx * KOOPAS_DIE_X_SPEED;
							vy = -KOOPAS_HIT_Y_SPEED;
							SetState(KOOPAS_STATE_HIDE);
						}
					}

					// Red Koopas do not leave the platform
					if (type == KOOPAS_RED)
					{
						// Case Brick
						if (dynamic_cast<CBrick*>(e->obj))
						{
							CBrick* brick = dynamic_cast<CBrick*>(e->obj);

							if (!brick->GetIsColLeft())
							{
								if (left < e->obj->left - KOOPAS_BBOX_WIDTH / 2)
								{
									x = e->obj->left - KOOPAS_BBOX_WIDTH / 2;
									vx = -vx;
								}
							}
							if (!brick->GetIsColRight())
							{
								if (right > e->obj->right + KOOPAS_BBOX_WIDTH / 2)
								{
									x = e->obj->right - KOOPAS_BBOX_WIDTH / 2;
									vx = -vx;
								}
							}
						}
						// Case others
						else
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
				}
			}
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CKoopas::Render()
{
	int ani = KOOPAS_ANI_WALKING_LEFT;
	if (type == KOOPAS_GREEN_WING || type == KOOPAS_RED_WING)
	{
		if (vx > 0)
			xReverse = true;
		else
			xReverse = false;
	}
	else if (type == KOOPAS_GREEN)
		xReverse = false;
	if (type == KOOPAS_GREEN_WING || type == KOOPAS_RED_WING)
	{
		/*if (vx > 0)
			xReverse = true;
		else
			xReverse = false;*/
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
	if (state == ENEMY_STATE_RESPAWN)
	{
		CGameObject::SetState(KOOPAS_STATE_WALKING);

		yReverse = false;
		if (type == KOOPAS_GREEN_WING)
			vx = -((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer()->nx * KOOPAS_WALKING_WING_SPEED;
		else if (type == KOOPAS_RED_WING)
			vy = 0.05f;
		else
		{
			if (vxSpawn == KOOPAS_WALKING_WING_SPEED)
			{
				type = KOOPAS_GREEN_WING;
				vx = -((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer()->nx * KOOPAS_WALKING_WING_SPEED;
			}
			else
				vx = -((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer()->nx * KOOPAS_WALKING_SPEED;
		}
		return;
	}

	CGameObject::SetState(state);

	switch (state)
	{
	case KOOPAS_STATE_DIE:
		AddPoint();
		if (type == KOOPAS_GREEN_WING)
			type = KOOPAS_GREEN;
		else if (type == KOOPAS_RED_WING)
			type = KOOPAS_RED;
		vy = -KOOPAS_DIE_Y_SPEED;
		yReverse = true;
		isDie = true;
		break;
	case KOOPAS_STATE_HIDE:
		if (type == KOOPAS_GREEN_WING)
			type = KOOPAS_GREEN;
		else if (type == KOOPAS_RED_WING)
			type = KOOPAS_RED;
		hide_start = GetTickCount();
		break;
	case KOOPAS_STATE_SPIN:
		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer()->canMultiScoreLand = true;
		break;
	case KOOPAS_STATE_WALKING:
		yReverse = false;
		if (type == KOOPAS_GREEN_WING)
			vx = -KOOPAS_WALKING_WING_SPEED;
		else if (type == KOOPAS_RED_WING)
			vx = 0;
		else
			vx = -KOOPAS_WALKING_SPEED;
		break;
	case KOOPAS_STATE_HOLD:
		if (isInGrid)
		{
			isInGrid = false;
			CGame::GetInstance()->GetCurrentScene()->GetBehindObjs()->push_back(this);
		}
		break;
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

void CKoopas::SetFlyRegion(float min, float max)
{
	yMin = min;
	yMax = max;
}

void CKoopas::UpdateDirection()
{
	if (state == KOOPAS_STATE_HOLD)
	{
		CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		nx = mario->nx;
	}
	else
		CGameObject::UpdateDirection();
}

void CKoopas::OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs)
{
	// Goomba
	if (dynamic_cast<CGoomba*>(obj))
	{
		if (state == KOOPAS_STATE_SPIN || state == KOOPAS_STATE_HOLD)
		{
			CGoomba* goomba = dynamic_cast<CGoomba*>(obj);
			goomba->vx = nx * ENEMY_DIE_X_SPEED;
			goomba->vy = -ENEMY_DIE_Y_SPEED;
			goomba->SetState(GOOMBA_STATE_DIE_REVERSE);

			if (state == KOOPAS_STATE_HOLD)
			{
				vx = nx * KOOPAS_DIE_X_SPEED;
				SetState(KOOPAS_STATE_DIE);
			}
		}
	}
	// Para Goomba
	else if (dynamic_cast<CParaGoomba*>(obj))
	{
		if (state == KOOPAS_STATE_SPIN || state == KOOPAS_STATE_HOLD)
		{
			CParaGoomba* para = dynamic_cast<CParaGoomba*>(obj);
			para->vx = nx * ENEMY_DIE_X_SPEED;
			para->vy = -ENEMY_DIE_Y_SPEED;
			para->SetState(PARA_GOOMBA_STATE_DIE_REVERSE);

			if (state == KOOPAS_STATE_HOLD)
			{
				vx = nx * KOOPAS_DIE_X_SPEED;
				SetState(KOOPAS_STATE_DIE);
			}
		}
	}
	// Koopas
	else if (dynamic_cast<CKoopas*>(obj))
	{
		CKoopas* koopas = dynamic_cast<CKoopas*>(obj);
		if (state == KOOPAS_STATE_SPIN || state == KOOPAS_STATE_HOLD)
		{
			if (!koopas->isDie)
			{
				koopas->vx = nx * KOOPAS_DIE_X_SPEED;
				koopas->SetState(KOOPAS_STATE_DIE);
			}

			if (state == KOOPAS_STATE_HOLD)
			{
				vx = nx * KOOPAS_DIE_X_SPEED;
				SetState(KOOPAS_STATE_DIE);
			}
		}
	}
	// Piranha
	else if (dynamic_cast<CPiranha*>(obj))
	{
		CPiranha* piranha = dynamic_cast<CPiranha*>(obj);
		if (state == KOOPAS_STATE_SPIN || state == KOOPAS_STATE_HOLD)
		{
			piranha->SetState(PIRANHA_STATE_DIE);
		}

		if (state == KOOPAS_STATE_HOLD)
		{
			vx = nx * KOOPAS_DIE_X_SPEED;
			SetState(KOOPAS_STATE_DIE);
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