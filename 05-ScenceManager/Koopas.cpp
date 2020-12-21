#include "Koopas.h"
#include "QuestionBrick.h"
#include "Game.h"
#include "PlayScence.h"

CKoopas::CKoopas(int type)
{
	switch (type)
	{
	case 1:
		this->type = KOOPAS_GREEN;
		break;
	case 2:
		this->type = KOOPAS_RED;
		break;
	default:
		break;
	}

	/*if (min != -1 && max != -1)
	{
		xMin = min;
		xMax = max;
	}*/

	SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_HIDE || state == KOOPAS_STATE_SPIN || state == KOOPAS_STATE_HOLD)
		bottom = y + KOOPAS_BBOX_HEIGHT_HIDE;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;

	CGameObject::GetBoundingBox(left, top, right, bottom);
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{	
	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;
	// Simple fall down
	vy += KOOPAS_GRAVITY * dt;

	if (y + KOOPAS_BBOX_HEIGHT_HIDE > 432)
	{
		isDie = true;
		isDead = true;
	}

	if (state == KOOPAS_STATE_HOLD)
		vy = 0;

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
				PreventMoveY(e->obj);

				if (state == KOOPAS_STATE_HIDE && yReverse == true)
				{
					vx = 0;
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
				if (state == KOOPAS_STATE_SPIN || state == KOOPAS_STATE_HOLD)
				{					
					if (goomba->state != GOOMBA_STATE_DIE_REVERSE)
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
				else
				{
					PreventMoveX(nx, goomba);
					vx = -vx;
				}
			}
			else if (dynamic_cast<CKoopas*>(e->obj))
			{
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				if (state == KOOPAS_STATE_SPIN)
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
				else
				{
					PreventMoveX(nx, koopas);
					vx = -vx;
				}
			}
			else if (dynamic_cast<CBox*>(e->obj) || dynamic_cast<CGround*>(e->obj) || dynamic_cast<CWarpPipe*>(e->obj) || dynamic_cast<CBrick*>(e->obj))
			{
				if (e->nx != 0)
				{
					if (!dynamic_cast<CBox*>(e->obj))
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
										CScene* scence = CGame::GetInstance()->GetCurrentScene();
										CMario* mario = ((CPlayScene*)scence)->GetPlayer();
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
						}

						PreventMoveX(nx, e->obj);
						vx = -vx;
					}
				}
				if (e->ny < 0 && type == KOOPAS_RED && state == KOOPAS_STATE_WALKING)
				{
					if (left <= e->obj->left - KOOPAS_BBOX_WIDTH / 2)
					{
						x = e->obj->left - KOOPAS_BBOX_WIDTH / 2;
						vx = -vx;
					}
					if (right >= e->obj->right + KOOPAS_BBOX_WIDTH / 2)
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
	if (state == KOOPAS_STATE_HIDE || state == KOOPAS_STATE_HOLD || state == KOOPAS_STATE_DIE) {
		ani = KOOPAS_ANI_HIDE;
	}
	else if (state == KOOPAS_STATE_SPIN) {
		if (vx > 0) ani = KOOPAS_ANI_SPIN_RIGHT;
		else if (vx <= 0) ani = KOOPAS_ANI_SPIN_LEFT;
	}
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
		yReverse = true;
		isDie = true;
		//die_start = GetTickCount();
		break;
		//case KOOPAS_STATE_HIDE:
		//	//y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_HIDE + 1;
		//	vx = 0;
		//	vy = 0;
		//	break;
		//	/*case KOOPAS_STATE_SPIN:
		//		vx = -KOOPAS_SPIN_SPEED;
		//		break;*/
	case KOOPAS_STATE_WALKING:
		vx = -KOOPAS_WALKING_SPEED;
		break;
		/*case KOOPAS_STATE_HOLD:
			vx = -KOOPAS_WALKING_SPEED;
			break;*/
	}

}