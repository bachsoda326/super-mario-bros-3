#include <algorithm>
#include <assert.h>
#include "Mario.h"
#include "Utils.h"
#include "Game.h"
#include "Portal.h"
#include "Box.h"
#include "CloudTooth.h"
#include "ParaGoomba.h"
#include "BreakableBrick.h"
#include "PSwitch.h"
#include "PlayScence.h"
#include "PlayerInfo.h"
#include "Point.h"

CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_SMALL;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;

	bullets = new vector<CBullet*>();
	for (int i = 0; i < 2; i++)
	{
		CBullet* bullet = new CBullet();
		//bullet->SetAnimationSet(ani_set);
		bullets->push_back(bullet);
		CGame::GetInstance()->GetCurrentScene()->GetFrontObjs()->push_back(bullet);
	}

	SetBoundingBox();
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Get others instance
	CCamera* camera = CCamera::GetInstance();
	int leftMap = camera->GetLeftMap();
	int topMap = camera->GetTopMap();
	int rightMap = camera->GetRightMap();
	int bottomMap = camera->GetBottomMap();
	int heightMap = camera->GetHeightMap();
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

	//DebugOut(L"VY: %f\n", vy);
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	if (state != MARIO_STATE_PIPE)
		vy += MARIO_GRAVITY * dt;

	// Max jump
	if (vy <= -0.2f)
	{
		vy = -0.2f;
		canJumpHigher = false;
	}

	// Die
	if (state == MARIO_STATE_DIE)
	{
		x += dx;
		y += dy;

		DecreasePower();

		if (GetTickCount() - die_start > 3000)
		{
			CPlayerInfo::GetInstance()->AdjustLife(-LIFE_1);
			CGame::GetInstance()->SwitchScene(WORLD_MAP_1);
		}

		return;
	}

	// Mario when col edge map
	if (x <= leftMap) x = leftMap;
	if (state == MARIO_STATE_FLY && y <= topMap) y = topMap;
	if (state != MARIO_STATE_END_SCENE && x + MARIO_BIG_BBOX_WIDTH >= rightMap) x = rightMap - MARIO_BIG_BBOX_WIDTH;
	if (!isOnOtherMap)
	{
		if (y > topMap + heightMap) SetState(MARIO_STATE_DIE);
	}

	// Camera push Mario in 1-4
	if (scene->GetMap()->GetId() == MAP_1_4)
	{
		if (x < camera->GetPosition().x - CGame::GetInstance()->GetScreenWidth() / 2)
			x = camera->GetPosition().x - CGame::GetInstance()->GetScreenWidth() / 2;
	}

	// Remove koopas when kick or throw
	if (koopas != NULL && koopas->state == KOOPAS_STATE_DIE)
	{
		koopas = NULL;
		isHold = false;
	}

	// Giao nhau vs obj
	for (int i = 0; i < coObjects->size(); i++)
	{
		if (AABBCheck(this, coObjects->at(i)))
		{
			OnIntersect(coObjects->at(i), coObjects);
		}
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

	// End scene
	if (state == MARIO_STATE_END_SCENE)
	{
		if (isOnGround)
			vx = 0.1f;
		if (x >= RIGHT_MAP_1_1)
		{
			isEndScene = true;
			vx = 0;
		}
	}

	if (!isOnGround && state != MARIO_STATE_FLY && state != MARIO_STATE_RUNJUMP)
	{
		DecreasePower();
	}

	if (kick_start != 0)
	{
		if ((GetTickCount() - kick_start) < 100)
		{
			SetState(MARIO_STATE_KICK);
		}
		else if ((GetTickCount() - kick_start) >= 100)
		{
			kick_start = 0;
		}
	}

	/*if (tail_start != 0 && (GetTickCount() - tail_start) < 5000)
	{
		SetState(MARIO_STATE_TAIL);
	}
	else
	{
		if (tail_start != 0 && (GetTickCount() - tail_start) >= 5000)
		{
			tail_start = 0;
			canHit = true;
		}
		if (state == MARIO_STATE_TAIL && !isOnGround)
			SetState(MARIO_STATE_JUMP_HIGH);
		canAttack = true;
	}*/

	if (tail_start != 0)
	{
		if (GetTickCount() - tail_start < 210)
			SetState(MARIO_STATE_TAIL);
		else
		{
			tail_start = 0;
			canHit = true;
			canAttack = true;

			if (!isOnGround)
				SetState(MARIO_STATE_JUMP_HIGH);
			else if (vx == 0)
				SetState(MARIO_STATE_IDLE);
			else
				SetState(MARIO_STATE_WALKING);
		}
	}

	if (fire_start != 0)
	{
		if ((GetTickCount() - fire_start) < 150)
		{
			if (isOnGround)
				SetState(MARIO_STATE_SHOT);
			else
				SetState(MARIO_STATE_JUMP_SHOT);
		}
		else
		{
			fire_start = 0;
			canAttack = true;
			if (state == MARIO_STATE_JUMP_SHOT)
				SetState(MARIO_STATE_JUMP_HIGH);
			else if (state == MARIO_STATE_RUNJUMP_SHOT)
				SetState(MARIO_STATE_RUNJUMP);
		}
	}

	if ((GetTickCount() - wag_start) < 300)
	{
		SetState(MARIO_STATE_WAG);
	}
	else if (state == MARIO_STATE_WAG)
	{
		SetState(MARIO_STATE_JUMP_HIGH);
	}
	if ((GetTickCount() - fly_start) < 300)
	{
		SetState(MARIO_STATE_FLY);
	}
	else if (state == MARIO_STATE_FLY)
	{
		SetState(MARIO_STATE_RUNJUMP);
	}
	if (state == MARIO_STATE_FLY && (GetTickCount() - fly_limit_start) > 5000)
	{
		//canJump = false;
		SetState(MARIO_STATE_JUMP_HIGH);
	}

	// Pipe down
	if ((GetTickCount() - pipe_down_start) < 1000)
	{
		SetState(MARIO_STATE_PIPE);
	}
	if (pipe_down_start != 0 && (GetTickCount() - pipe_down_start) >= 1000)
	{
		pipe_down_start = 0;

		if (isToPipeDown)
		{
			SetState(MARIO_STATE_JUMP_HIGH);
			scene->ChangeMarioLocation(true, true, pipe_tele_x, pipe_tele_y);
		}
		else
		{
			pipe_up_start = GetTickCount();
			if (level == MARIO_LEVEL_SMALL)
				scene->ChangeMarioLocation(true, true, pipe_tele_x, pipe_tele_y + MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);
			else
				scene->ChangeMarioLocation(true, true, pipe_tele_x, pipe_tele_y);
		}
	}
	// Pipe up
	if (isToPipeDown)
	{
		if ((GetTickCount() - pipe_up_start) < 1500)
		{
			SetState(MARIO_STATE_PIPE);
		}
		if (pipe_up_start != 0)
		{
			if ((GetTickCount() - pipe_up_start) == 1500)
			{
				//SetState(MARIO_STATE_IDLE);
				if (level == MARIO_LEVEL_SMALL)
					scene->ChangeMarioLocation(false, true, pipe_tele_x, pipe_tele_y + MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);
				else
					scene->ChangeMarioLocation(false, true, pipe_tele_x, pipe_tele_y);
			}
			else if ((GetTickCount() - pipe_up_start) >= 2500)
			{
				pipe_up_start = 0;
				SetState(MARIO_STATE_IDLE);
			}
		}
	}
	else
	{
		if ((GetTickCount() - pipe_up_start) < 1000)
		{
			SetState(MARIO_STATE_PIPE);
		}
		if (pipe_up_start != 0 && (GetTickCount() - pipe_up_start) >= 1000)
		{
			pipe_up_start = 0;
			SetState(MARIO_STATE_IDLE);
		}
	}
	if ((GetTickCount() - eat_item_start) < 1)
	{
		SetState(MARIO_STATE_EAT_ITEM);
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

		if (vy != 0)
		{
			isOnGround = false;
			if (vy > 0)
				canJump = false;
		}
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		colX = nx;

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		/*if (nx != 0)
			vx = 0;*/
			//if (ny != 0)
			//{
			//	vy = 0;
			//	//y += min_ty * dy + ny * 0.4f;
			//}

		/*if (ny < 0 && vy >= 0)
		{
			isOnGround = true;
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
		else if (ny > 0)
		{
			vy = 0;
			canJump = false;
			canJumpHigher = false;
		}*/
		/*else if (nx == 0 && ny == 0)
			isOnGround = false;*/

			//
			// Collision logic with other objects
			//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			e->obj->colX = e->nx;
			e->obj->colY = e->ny;

			// Stand on obj
			if (ny < 0 && e->obj != NULL)
			{
				if (canMultiScoreJump && isOnGround)
				{
					canMultiScoreJump = false;
					pointFactor = 0;
				}
				if (!dynamic_cast<CCoin*>(e->obj) && !dynamic_cast<CBullet*>(e->obj) && !dynamic_cast<CLeaf*>(e->obj) && !dynamic_cast<CMushRoom*>(e->obj) && !dynamic_cast<CPiranha*>(e->obj))
				{
					if (!(dynamic_cast<CBreakableBrick*>(e->obj) && e->obj->GetState() == BREAKABLE_BRICK_STATE_COIN || dynamic_cast<CPSwitch*>(e->obj) && e->obj->GetState() == PSWITCH_STATE_HIT))
					{
						if (state != MARIO_STATE_DIE && !dynamic_cast<CFlyBar*>(e->obj))
							vy = 0;
						if (state != MARIO_STATE_EAT_ITEM)
						{
							if (state == MARIO_STATE_DUCK)
								y = e->obj->y - (bottom - top + MARIO_BIG_BBOX_HEIGHT_DUCK / 2);
							else
								y = e->obj->y - (bottom - top);
						}
					}
				}
			}

			// Logic with jump
			if (ny < 0 && vy >= 0)
			{
				isOnGround = true;
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
			else if (ny > 0 && !dynamic_cast<CFlyBar*>(e->obj) && !dynamic_cast<CCoin*>(e->obj) && !dynamic_cast<CLeaf*>(e->obj) && !dynamic_cast<CBullet*>(e->obj) && !dynamic_cast<CMushRoom*>(e->obj) && !dynamic_cast<CCloudTooth*>(e->obj) && !dynamic_cast<CKoopas*>(e->obj) && !dynamic_cast<CGoomba*>(e->obj))
			{
				if (!((dynamic_cast<CBreakableBrick*>(e->obj) && e->obj->GetState() == BREAKABLE_BRICK_STATE_COIN) || ((dynamic_cast<CWarpPipe*>(e->obj)/* && state == MARIO_STATE_PIPE*/))))
				{
					vy = 0;
					canJump = false;
					canJumpHigher = false;
				}
			}

			/*if (dynamic_cast<CBullet*>(e->obj) || dynamic_cast<CMushRoom*>(e->obj) || dynamic_cast<CLeaf*>(e->obj))
			{
				x += dx;
				y += dy;
			}*/
			if (dynamic_cast<CEnemy*>(e->obj))
			{
				//canRepeatJump = true;
				canJump = true;
				canJumpHigher = true;
			}
			// Fly Bar
			if (dynamic_cast<CFlyBar*>(e->obj))
			{
				CFlyBar* bar = dynamic_cast<CFlyBar*>(e->obj);
				/*if (e->nx != 0)
					PreventMoveX(-1, e->obj);*/
				if (e->ny > 0)
					y += dy;
				else if (e->ny < 0)
				{
					if (bar->GetState() != FLY_BAR_STATE_FALL)
						bar->SetState(FLY_BAR_STATE_FALL);
					else
					{
						if (vx > 0 && left + (right - left) / 4 >= bar->right || vx < 0 && right - (right - left) / 4 <= bar->left)
							vy = 0;
					}
					/*vy = bar->vy;*/
				}
			}
			// Cloud tooth
			if (dynamic_cast<CCloudTooth*>(e->obj))
			{
				if (state == MARIO_STATE_FLY || e->nx != 0)
					x += dx;
				if (e->ny > 0)
					y += dy;
			}
			// Bullet
			else if (dynamic_cast<CBullet*>(e->obj))
			{
				x += dx;
				y += dy;

				/*CBullet* bullet = dynamic_cast<CBullet*>(e->obj);

				if (bullet->isEnemy)
				{
					Hurt();
				}*/
			}
			// Piranha
			else if (dynamic_cast<CPiranha*>(e->obj))
			{
				x += dx;
				y += dy;

				/*CPiranha* piranha = dynamic_cast<CPiranha*>(e->obj);

				if (state != MARIO_STATE_TAIL && piranha->GetState() == PIRANHA_STATE_NORMAL)
				{
					Hurt();
				}*/
			}
			// Box
			else if (dynamic_cast<CBox*>(e->obj))
			{
				if (e->nx != 0)
					x += dx;
			}
			// Warp pipe
			else if (dynamic_cast<CWarpPipe*>(e->obj))
			{
				if (state == MARIO_STATE_PIPE)
				{
					y += dy;
				}

				CWarpPipe* pipe = dynamic_cast<CWarpPipe*>(e->obj);
				if (left > pipe->left && right < pipe->right)
				{
					switch (pipe->type)
					{
					case WARPPIPE_TYPE_DOWN:
						if (CGame::GetInstance()->IsKeyDown(DIK_DOWN))
						{
							isToPipeDown = true;
							pipe_tele_x = pipe->tele_x;
							pipe_tele_y = pipe->tele_y;
							SetState(MARIO_STATE_PIPE);
							pipe_down_start = GetTickCount();
							y += MARIO_PIPE_DOWN_HEIGHT;
						}
						break;
					case WARPPIPE_TYPE_PLANE:
						if (CGame::GetInstance()->IsKeyDown(DIK_DOWN))
						{
							isToPipeDown = false;
							pipe_tele_x = pipe->tele_x;
							pipe_tele_y = pipe->tele_y;
							SetState(MARIO_STATE_PIPE);
							pipe_down_start = GetTickCount();
							y += MARIO_PIPE_DOWN_HEIGHT;
							camera->SetIsMoving(false);
						}
						break;
					case WARPPIPE_TYPE_UP:
						if (CGame::GetInstance()->IsKeyDown(DIK_UP))
						{
							pipe_tele_x = pipe->tele_x;
							pipe_tele_y = pipe->tele_y;
							SetState(MARIO_STATE_PIPE);
							pipe_up_start = GetTickCount();
						}
						break;
					default:
						break;
					}
				}
			}
			// Goomba
			else if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				x += dx;
				y += dy;

				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (!goomba->isDie)
				{
					if (e->ny < 0)
					{
						canMultiScoreJump = true;
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
					/*else if (state != MARIO_STATE_TAIL && e->nx != 0)
					{
						Hurt();
					}*/
				}
			}
			// Para Goomba
			else if (dynamic_cast<CParaGoomba*>(e->obj)) // if e->obj is ParaGoomba 
			{
				x += dx;
				y += dy;

				CParaGoomba* para = dynamic_cast<CParaGoomba*>(e->obj);

				// jump on top >> kill Para Goomba and deflect a bit 
				if (!para->isDie)
				{
					if (e->ny < 0)
					{
						canMultiScoreJump = true;
						if (para->GetLevel() == PARA_GOOMBA_LEVEL_WING)
							para->SetState(PARA_GOOMBA_STATE_LOOSE_WING);
						else
							para->SetState(PARA_GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
					/*else if (e->nx != 0)
					{
						if (state == MARIO_STATE_TAIL)
						{
							para->vx = -nx * 0.05f;
							para->vy = -0.1f;
							para->SetState(PARA_GOOMBA_STATE_DIE_REVERSE);
						}
						else
						{
							Hurt();
						}
					}*/
				}
			}
			// Koopas
			else if (dynamic_cast<CKoopas*>(e->obj)) // if e->obj is CKoopas 
			{
				x += dx;
				y += dy;

				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);

				//if (koopas->GetState() == KOOPAS_STATE_HIDE)
				//{
				//	//koopas->SetPosition(x, y);
				//}
				// jump on top >> kill CKoopas, can kick or throw
				if (e->ny < 0)
				{
					canMultiScoreJump = true;
					koopas->AddPoint();
					if (koopas->type == KOOPAS_GREEN_WING)
					{
						koopas->type = KOOPAS_GREEN;
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
					else if (koopas->type == KOOPAS_RED_WING)
					{
						koopas->type = KOOPAS_RED;
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
					else if (koopas->GetState() == KOOPAS_STATE_HIDE)
					{
						koopas->SetState(KOOPAS_STATE_SPIN);
						koopas->vx = this->nx * KOOPAS_SPIN_SPEED;
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
					else
					{
						koopas->vx = 0;
						koopas->vy = 0;
						koopas->SetState(KOOPAS_STATE_HIDE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					/*if (state == MARIO_STATE_TAIL)
					{
						koopas->vx = -nx * 0.07f;
						koopas->vy = -0.5f;
						koopas->yReverse = true;
						koopas->SetState(KOOPAS_STATE_HIDE);
					}
					else if (koopas->GetState() == KOOPAS_STATE_WALKING || koopas->GetState() == KOOPAS_STATE_SPIN)
					{
						Hurt();
						if (untouchable == 0)
							koopas->vx = -koopas->vx;
					}
					else*/ if (state != MARIO_STATE_TAIL && koopas->GetState() == KOOPAS_STATE_HIDE)
					{
						if (canHold)
						{
							SetKoopas(koopas);
							koopas->SetState(KOOPAS_STATE_HOLD);
							/*SetState(MARIO_STATE_IDLE_HOLD);*/
							isHold = true;
						}
						else
						{
							SetState(MARIO_STATE_KICK);
							kick_start = GetTickCount();
							koopas->SetState(KOOPAS_STATE_SPIN);
							koopas->vx = this->nx * KOOPAS_SPIN_SPEED;
						}
					}
				}
			}
			// Portal
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				//CGame::GetInstance()->SwitchScene(p->GetSceneId());
				p->SetState(PORTAL_STATE_PICK_CARD);
				SetState(MARIO_STATE_END_SCENE);
			}
			// MushRoom
			else if (dynamic_cast<CMushRoom*>(e->obj))
			{
				/*x += dx;
				y += dy;*/
				CMushRoom* mushroom = dynamic_cast<CMushRoom*>(e->obj);
				if (!mushroom->isDie)
				{
					EatMushRoom(mushroom, coObjects);
				}
			}
			// Leaf
			else if (dynamic_cast<CLeaf*>(e->obj))
			{
				/*x += dx;
				y += dy;*/
				CLeaf* leaf = dynamic_cast<CLeaf*>(e->obj);
				if (!leaf->isDie)
				{
					EatLeaf(leaf, coObjects);
				}
			}
			// Coin
			else if (dynamic_cast<CCoin*>(e->obj))
			{
				/*x += dx;
				y += dy;*/
				CCoin* coin = dynamic_cast<CCoin*>(e->obj);
				if (!coin->isDie)
				{
					EatCoin(coin, coObjects);
				}
			}
			// Question Brick
			else if (dynamic_cast<CQuestionBrick*>(e->obj))
			{
				if (e->ny > 0/* || (e->nx != 0 && state == MARIO_STATE_TAIL)*/)
				{
					CQuestionBrick* qBrick = dynamic_cast<CQuestionBrick*>(e->obj);
					if (qBrick->GetState() == QUESTION_BRICK_STATE_NORMAL)
					{
						qBrick->vy = -BRICK_Y_SPEED;
						switch (qBrick->type)
						{
						case BRICK_NORMAL:
							qBrick->SetState(QUESTION_BRICK_STATE_HIT_COIN);
							break;
						case BRICK_ITEM:
							switch (level)
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
			// Breakable Brick
			else if (dynamic_cast<CBreakableBrick*>(e->obj))
			{
				CBreakableBrick* bBrick = dynamic_cast<CBreakableBrick*>(e->obj);

				if (bBrick->GetState() == BREAKABLE_BRICK_STATE_COIN)
				{
					x += dx;
					y += dy;

					/*bBrick->DeleteFrontObjs(coObjects);*/
				}
				else if (bBrick->GetState() == QUESTION_BRICK_STATE_NORMAL)
				{
					if (e->ny > 0 /*|| (e->nx != 0 && state == MARIO_STATE_TAIL)*/)
					{
						switch (bBrick->type)
						{
						case BREAKABLE_BRICK_TYPE_1UP_MUSHROOM:
							if (level > MARIO_LEVEL_SMALL)
							{
								if (x <= bBrick->x)
									bBrick->SetState(BREAKABLE_BRICK_STATE_1UP_MUSHROOM_LEFT);
								else
									bBrick->SetState(BREAKABLE_BRICK_STATE_1UP_MUSHROOM_RIGHT);
							}
							break;
						case BREAKABLE_BRICK_TYPE_ITEM:
							switch (level)
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
							if (level > MARIO_LEVEL_SMALL)
							{
								bBrick->SetState(BREAKABLE_BRICK_STATE_HIT_MULTI_COIN);
								bBrick->vy = -0.1f;
							}
							break;
						case BREAKABLE_BRICK_TYPE_P_SWITCH:
							if (level > MARIO_LEVEL_SMALL)
							{
								bBrick->SetState(BREAKABLE_BRICK_STATE_P_SWITCH);
							}
							break;
						case BREAKABLE_BRICK_TYPE_COIN:
							if (level > MARIO_LEVEL_SMALL)
							{
								bBrick->SetState(BREAKABLE_BRICK_STATE_BREAK);
							}
							break;
						default:
							break;
						}
					}
				}
			}
			// P Switch
			else if (dynamic_cast<CPSwitch*>(e->obj))
			{
				if (e->ny < 0)
				{
					CPSwitch* pSwitch = dynamic_cast<CPSwitch*>(e->obj);
					switch (pSwitch->GetState())
					{
					case PSWITCH_STATE_NORMAL:
						pSwitch->SetState(PSWITCH_STATE_HIT);
						break;
					case PSWITCH_STATE_HIT:
						y += dy;
						break;
					default:
						break;
					}
				}
			}

			/*else if (!dynamic_cast<CGoomba*>(e->obj) && !dynamic_cast<CKoopas*>(e->obj))
			{
				if (e->nx != 0)
					if (state == MARIO_STATE_PREPARE_RUN || state == MARIO_STATE_RUN)
					{
						vx = this->nx * MARIO_WALKING_SPEED;
						SetState(MARIO_STATE_WALKING);
					}
			}*/
			if (isOnGround && (state == MARIO_STATE_PREPARE_RUN || state == MARIO_STATE_RUN))
			{
				if (!dynamic_cast<CBox*>(e->obj) && !dynamic_cast<CGoomba*>(e->obj) && !dynamic_cast<CKoopas*>(e->obj) && e->nx != 0)
				{
					vx = this->nx * MARIO_WALKING_SPEED;
					SetState(MARIO_STATE_WALKING);
				}
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
		if (state == MARIO_STATE_END_SCENE)
		{
			if (isOnGround)
				ani = MARIO_ANI_BIG_WALKING_RIGHT;
			else
				ani = MARIO_ANI_BIG_JUMP_RIGHT;
		}
		if (state == MARIO_STATE_IDLE || state == MARIO_STATE_EAT_ITEM || state == MARIO_STATE_TAIL || state == MARIO_STATE_SHOT || state == MARIO_STATE_JUMP_SHOT || state == MARIO_STATE_RUNJUMP_SHOT || state == MARIO_STATE_FLY || state == MARIO_STATE_WAG)
		{
			if (isHold)
			{
				if (nx > 0) ani = MARIO_ANI_BIG_IDLE_HOLD_RIGHT;
				else ani = MARIO_ANI_BIG_IDLE_HOLD_LEFT;
			}
			else
			{
				if (nx > 0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
				else ani = MARIO_ANI_BIG_IDLE_LEFT;
			}
		}
		if (state == MARIO_STATE_WALKING)
		{
			if (isHold)
			{
				if (nx > 0) ani = MARIO_ANI_BIG_HOLD_RIGHT;
				else ani = MARIO_ANI_BIG_HOLD_LEFT;
			}
			else
			{
				if (nx > 0) ani = MARIO_ANI_BIG_WALKING_RIGHT;
				else ani = MARIO_ANI_BIG_WALKING_LEFT;
			}
		}
		if (state == MARIO_STATE_JUMP_HIGH || state == MARIO_STATE_JUMP_SHORT)
		{
			if (isHold)
			{
				if (nx > 0) ani = MARIO_ANI_BIG_JUMP_HOLD_RIGHT;
				else ani = MARIO_ANI_BIG_JUMP_HOLD_LEFT;
			}
			else
			{
				if (nx > 0) ani = MARIO_ANI_BIG_JUMP_RIGHT;
				else ani = MARIO_ANI_BIG_JUMP_LEFT;
			}
		}
		if (state == MARIO_STATE_RUN)
		{
			if (nx > 0) ani = MARIO_ANI_BIG_RUN_RIGHT;
			else ani = MARIO_ANI_BIG_RUN_LEFT;
		}
		if (state == MARIO_STATE_PREPARE_RUN)
		{
			if (isHold)
			{
				if (nx > 0) ani = MARIO_ANI_BIG_RUN_HOLD_RIGHT;
				else ani = MARIO_ANI_BIG_RUN_HOLD_LEFT;
			}
			else
			{
				if (nx > 0) ani = MARIO_ANI_BIG_PREPARE_RUN_RIGHT;
				else ani = MARIO_ANI_BIG_PREPARE_RUN_LEFT;
			}
		}
		if (state == MARIO_STATE_RUNJUMP)
		{
			if (nx > 0) ani = MARIO_ANI_BIG_RUNJUMP_RIGHT;
			else ani = MARIO_ANI_BIG_RUNJUMP_LEFT;
		}
		/*if (state == MARIO_STATE_IDLE_HOLD)
		{
			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_ANI_BIG_IDLE_HOLD_RIGHT;
				else ani = MARIO_ANI_BIG_IDLE_HOLD_LEFT;
			}
			else
			{
				if (nx > 0) ani = MARIO_ANI_BIG_HOLD_RIGHT;
				else ani = MARIO_ANI_BIG_HOLD_LEFT;
			}
		}*/
		if (state == MARIO_STATE_KICK)
		{
			if (nx > 0) ani = MARIO_ANI_BIG_KICK_RIGHT;
			else ani = MARIO_ANI_BIG_KICK_LEFT;
		}
		if (state == MARIO_STATE_SKID)
		{
			if (nx > 0) ani = MARIO_ANI_BIG_SKID_RIGHT;
			else ani = MARIO_ANI_BIG_SKID_LEFT;
		}
		if (state == MARIO_STATE_DUCK)
		{
			if (nx > 0) ani = MARIO_ANI_BIG_DUCK_RIGHT;
			else ani = MARIO_ANI_BIG_DUCK_LEFT;
		}
		if (state == MARIO_STATE_PIPE)
		{
			ani = MARIO_ANI_BIG_PIPE;
		}
	}
	else if (level == MARIO_LEVEL_RACCOON)
	{
		if (state == MARIO_STATE_END_SCENE)
		{
			if (isOnGround)
				ani = MARIO_ANI_RACCOON_WALKING_RIGHT;
			else
				ani = MARIO_ANI_RACCOON_JUMP_RIGHT;
		}
		if (state == MARIO_STATE_IDLE || state == MARIO_STATE_EAT_ITEM)
		{
			if (isHold)
			{
				if (nx > 0) ani = MARIO_ANI_RACCOON_IDLE_HOLD_RIGHT;
				else ani = MARIO_ANI_RACCOON_IDLE_HOLD_LEFT;
			}
			else
			{
				if (nx > 0) ani = MARIO_ANI_RACCOON_IDLE_RIGHT;
				else ani = MARIO_ANI_RACCOON_IDLE_LEFT;
			}
		}
		if (state == MARIO_STATE_WALKING)
		{
			if (isHold)
			{
				if (nx > 0) ani = MARIO_ANI_RACCOON_HOLD_RIGHT;
				else ani = MARIO_ANI_RACCOON_HOLD_LEFT;
			}
			else
			{
				if (nx > 0) ani = MARIO_ANI_RACCOON_WALKING_RIGHT;
				else ani = MARIO_ANI_RACCOON_WALKING_LEFT;
			}
		}
		if (state == MARIO_STATE_JUMP_HIGH || state == MARIO_STATE_JUMP_SHORT)
		{
			if (isHold)
			{
				if (nx > 0) ani = MARIO_ANI_RACCOON_JUMP_HOLD_RIGHT;
				else ani = MARIO_ANI_RACCOON_JUMP_HOLD_LEFT;
			}
			else
			{
				if (nx > 0) ani = MARIO_ANI_RACCOON_JUMP_RIGHT;
				else ani = MARIO_ANI_RACCOON_JUMP_LEFT;
			}
		}
		if (state == MARIO_STATE_RUN)
		{
			if (nx > 0) ani = MARIO_ANI_RACCOON_RUN_RIGHT;
			else ani = MARIO_ANI_RACCOON_RUN_LEFT;
		}
		if (state == MARIO_STATE_PREPARE_RUN)
		{
			if (isHold)
			{
				if (nx > 0) ani = MARIO_ANI_RACCOON_RUN_HOLD_RIGHT;
				else ani = MARIO_ANI_RACCOON_RUN_HOLD_LEFT;
			}
			else
			{
				if (nx > 0) ani = MARIO_ANI_RACCOON_PREPARE_RUN_RIGHT;
				else ani = MARIO_ANI_RACCOON_PREPARE_RUN_LEFT;
			}
		}
		if (state == MARIO_STATE_RUNJUMP)
		{
			if (nx > 0) ani = MARIO_ANI_RACCOON_RUNJUMP_RIGHT;
			else ani = MARIO_ANI_RACCOON_RUNJUMP_LEFT;
		}
		if (state == MARIO_STATE_FLY)
		{
			if (nx > 0) ani = MARIO_ANI_RACCOON_FLY_RIGHT;
			else ani = MARIO_ANI_RACCOON_FLY_LEFT;
		}
		/*if (state == MARIO_STATE_IDLE_HOLD)
		{
			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_ANI_RACCOON_IDLE_HOLD_RIGHT;
				else ani = MARIO_ANI_RACCOON_IDLE_HOLD_LEFT;
			}
			else
			{
				if (nx > 0) ani = MARIO_ANI_RACCOON_HOLD_RIGHT;
				else ani = MARIO_ANI_RACCOON_HOLD_LEFT;
			}
		}*/
		if (state == MARIO_STATE_TAIL)
		{
			if (nx > 0) ani = MARIO_ANI_RACCOON_TAIL_RIGHT;
			else ani = MARIO_ANI_RACCOON_TAIL_LEFT;
		}
		if (state == MARIO_STATE_KICK)
		{
			if (nx > 0) ani = MARIO_ANI_RACCOON_KICK_RIGHT;
			else ani = MARIO_ANI_RACCOON_KICK_LEFT;
		}
		if (state == MARIO_STATE_SKID)
		{
			if (nx > 0) ani = MARIO_ANI_RACCOON_SKID_RIGHT;
			else ani = MARIO_ANI_RACCOON_SKID_LEFT;
		}
		if (state == MARIO_STATE_DUCK)
		{
			if (nx > 0) ani = MARIO_ANI_RACCOON_DUCK_RIGHT;
			else ani = MARIO_ANI_RACCOON_DUCK_LEFT;
		}
		if (state == MARIO_STATE_WAG)
		{
			if (nx > 0) ani = MARIO_ANI_RACCOON_WAG_RIGHT;
			else ani = MARIO_ANI_RACCOON_WAG_LEFT;
		}
		if (state == MARIO_STATE_PIPE)
		{
			ani = MARIO_ANI_RACCOON_PIPE;
		}
	}
	else if (level == MARIO_LEVEL_FIRE)
	{
		if (state == MARIO_STATE_END_SCENE)
		{
			if (isOnGround)
				ani = MARIO_ANI_FIRE_WALKING_RIGHT;
			else
				ani = MARIO_ANI_FIRE_JUMP_RIGHT;
		}
		if (state == MARIO_STATE_IDLE || state == MARIO_STATE_EAT_ITEM)
		{
			if (isHold)
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_IDLE_HOLD_RIGHT;
				else ani = MARIO_ANI_FIRE_IDLE_HOLD_LEFT;
			}
			else
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_IDLE_RIGHT;
				else ani = MARIO_ANI_FIRE_IDLE_LEFT;
			}
		}
		if (state == MARIO_STATE_WALKING)
		{
			if (isHold)
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_HOLD_RIGHT;
				else ani = MARIO_ANI_FIRE_HOLD_LEFT;
			}
			else
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_WALKING_RIGHT;
				else ani = MARIO_ANI_FIRE_WALKING_LEFT;
			}
		}
		if (state == MARIO_STATE_JUMP_HIGH || state == MARIO_STATE_JUMP_SHORT)
		{
			if (isHold)
			{
				/*if (nx > 0) ani = MARIO_ANI_FIRE_JUMP_HOLD_RIGHT;
				else ani = MARIO_ANI_FIRE_JUMP_HOLD_LEFT;*/
				if (nx > 0) ani = MARIO_ANI_FIRE_JUMP_RIGHT;
				else ani = MARIO_ANI_FIRE_JUMP_LEFT;
			}
			else
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_JUMP_RIGHT;
				else ani = MARIO_ANI_FIRE_JUMP_LEFT;
			}
		}
		if (state == MARIO_STATE_RUN)
		{
			if (nx > 0) ani = MARIO_ANI_FIRE_RUN_RIGHT;
			else ani = MARIO_ANI_FIRE_RUN_LEFT;
		}
		if (state == MARIO_STATE_PREPARE_RUN)
		{
			if (isHold)
			{
				/*if (nx > 0) ani = MARIO_ANI_FIRE_RUN_HOLD_RIGHT;
				else ani = MARIO_ANI_FIRE_RUN_HOLD_LEFT;*/
				if (nx > 0) ani = MARIO_ANI_FIRE_PREPARE_RUN_RIGHT;
				else ani = MARIO_ANI_FIRE_PREPARE_RUN_LEFT;
			}
			else
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_PREPARE_RUN_RIGHT;
				else ani = MARIO_ANI_FIRE_PREPARE_RUN_LEFT;
			}
		}
		if (state == MARIO_STATE_RUNJUMP || state == MARIO_STATE_JUMP_SHOT)
		{
			if (nx > 0) ani = MARIO_ANI_FIRE_RUNJUMP_RIGHT;
			else ani = MARIO_ANI_FIRE_RUNJUMP_LEFT;
		}
		/*if (state == MARIO_STATE_IDLE_HOLD)
		{
			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_IDLE_HOLD_RIGHT;
				else ani = MARIO_ANI_FIRE_IDLE_HOLD_LEFT;
			}
			else
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_HOLD_RIGHT;
				else ani = MARIO_ANI_FIRE_HOLD_LEFT;
			}
		}*/
		if (state == MARIO_STATE_KICK)
		{
			if (nx > 0) ani = MARIO_ANI_FIRE_KICK_RIGHT;
			else ani = MARIO_ANI_FIRE_KICK_LEFT;
		}
		if (state == MARIO_STATE_SKID)
		{
			if (nx > 0) ani = MARIO_ANI_FIRE_SKID_RIGHT;
			else ani = MARIO_ANI_FIRE_SKID_LEFT;
		}
		if (state == MARIO_STATE_DUCK)
		{
			if (nx > 0) ani = MARIO_ANI_FIRE_DUCK_RIGHT;
			else ani = MARIO_ANI_FIRE_DUCK_LEFT;
		}
		if (state == MARIO_STATE_SHOT)
		{
			if (nx > 0) ani = MARIO_ANI_FIRE_THROW_RIGHT;
			else ani = MARIO_ANI_FIRE_THROW_LEFT;
		}
		if (state == MARIO_STATE_PIPE)
		{
			ani = MARIO_ANI_FIRE_PIPE;
		}
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		if (state == MARIO_STATE_END_SCENE)
		{
			if (isOnGround)
				ani = MARIO_ANI_SMALL_WALKING_RIGHT;
			else
				ani = MARIO_ANI_SMALL_JUMP_RIGHT;
		}
		if (state == MARIO_STATE_IDLE || state == MARIO_STATE_EAT_ITEM || state == MARIO_STATE_TAIL || state == MARIO_STATE_SHOT || state == MARIO_STATE_JUMP_SHOT || state == MARIO_STATE_RUNJUMP_SHOT || state == MARIO_STATE_FLY)
		{
			if (isHold)
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_HOLD_RIGHT;
				else ani = MARIO_ANI_SMALL_IDLE_HOLD_LEFT;
			}
			else
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
				else ani = MARIO_ANI_SMALL_IDLE_LEFT;
			}
		}
		if (state == MARIO_STATE_WALKING)
		{
			if (isHold)
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_HOLD_RIGHT;
				else ani = MARIO_ANI_SMALL_HOLD_LEFT;
			}
			else
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_WALKING_RIGHT;
				else ani = MARIO_ANI_SMALL_WALKING_LEFT;
			}
		}
		if (state == MARIO_STATE_JUMP_HIGH || state == MARIO_STATE_JUMP_SHORT)
		{
			if (isHold)
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_JUMP_HOLD_RIGHT;
				else ani = MARIO_ANI_SMALL_JUMP_HOLD_LEFT;
			}
			else
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_JUMP_RIGHT;
				else ani = MARIO_ANI_SMALL_JUMP_LEFT;
			}
		}
		if (state == MARIO_STATE_RUN)
		{
			if (nx > 0) ani = MARIO_ANI_SMALL_RUN_RIGHT;
			else ani = MARIO_ANI_SMALL_RUN_LEFT;
		}
		if (state == MARIO_STATE_PREPARE_RUN)
		{
			if (isHold)
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_RUN_HOLD_RIGHT;
				else ani = MARIO_ANI_SMALL_RUN_HOLD_LEFT;
			}
			else
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_PREPARE_RUN_RIGHT;
				else ani = MARIO_ANI_SMALL_PREPARE_RUN_LEFT;
			}
		}
		if (state == MARIO_STATE_RUNJUMP)
		{
			if (nx > 0) ani = MARIO_ANI_SMALL_RUNJUMP_RIGHT;
			else ani = MARIO_ANI_SMALL_RUNJUMP_LEFT;
		}
		/*if (state == MARIO_STATE_IDLE_HOLD)
		{
			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_HOLD_RIGHT;
				else ani = MARIO_ANI_SMALL_IDLE_HOLD_LEFT;
			}
			else
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_HOLD_RIGHT;
				else ani = MARIO_ANI_SMALL_HOLD_LEFT;
			}
		}*/
		if (state == MARIO_STATE_KICK)
		{
			if (nx > 0) ani = MARIO_ANI_SMALL_KICK_RIGHT;
			else ani = MARIO_ANI_SMALL_KICK_LEFT;
		}
		if (state == MARIO_STATE_SKID)
		{
			if (nx > 0) ani = MARIO_ANI_SMALL_SKID_RIGHT;
			else ani = MARIO_ANI_SMALL_SKID_LEFT;
		}
		if (state == MARIO_STATE_PIPE)
		{
			ani = MARIO_ANI_SMALL_PIPE;
		}
	}

	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y, xReverse, yReverse, false, alpha);

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
			/*case MARIO_STATE_JUMP_HIGH:
				if (canJumpHigher)
					vy -= MARIO_JUMP_HIGH_SPEED_Y;
				break;*/
	case MARIO_STATE_JUMP_SHORT:
		vy = -MARIO_JUMP_SHORT_SPEED_Y;
		break;
	case MARIO_STATE_IDLE:
		vx = 0;
		break;
	case MARIO_STATE_DIE:
		vx = 0;
		vy = -MARIO_DIE_DEFLECT_SPEED;
		die_start = GetTickCount();
		break;
	case MARIO_STATE_WAG:
		vy = 0.05f;
		break;
	case MARIO_STATE_FLY:
		vy = -0.08f;
		break;
	case MARIO_STATE_RUN:
		vx = nx * MARIO_RUN_SPEED;
		break;
	case MARIO_STATE_PIPE:
		vx = 0;
		if (pipe_down_start != 0)
			vy = 0.02f;
		else if (pipe_up_start != 0)
			vy = -0.02f;
		break;
	case MARIO_STATE_DUCK:
		canAttack = false;
		break;
	case MARIO_STATE_END_SCENE:
		vx = 0;
		vy = 0;
		break;
	}
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == MARIO_STATE_DUCK)
		top = y + MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT_DUCK;
	else
		top = y;

	if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_RACCOON || level == MARIO_LEVEL_FIRE)
	{
		/*if (state == MARIO_STATE_TAIL)
		{
			left = x - 8;
			right = x + MARIO_BIG_BBOX_WIDTH + 8;
		}
		else
		{
			left = x;
			right = left + MARIO_BIG_BBOX_WIDTH;
		}*/
		left = x;
		right = left + MARIO_BIG_BBOX_WIDTH;
		if (state == MARIO_STATE_DUCK)
			bottom = top + MARIO_BIG_BBOX_HEIGHT_DUCK;
		else
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
	}
	else
	{
		left = x;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}

	CGameObject::GetBoundingBox(left, top, right, bottom);
}

void CMario::SetBoundingBox()
{
	if (state == MARIO_STATE_DUCK)
		top = y + MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT_DUCK;
	else
		top = y;

	if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_RACCOON || level == MARIO_LEVEL_FIRE)
	{
		/*if (state == MARIO_STATE_TAIL)
		{
			left = x - 8;
			right = x + MARIO_BIG_BBOX_WIDTH + 8;
		}
		else
		{
			left = x;
			right = left + MARIO_BIG_BBOX_WIDTH;
		}*/
		left = x;
		right = left + MARIO_BIG_BBOX_WIDTH;
		if (state == MARIO_STATE_DUCK)
			bottom = top + MARIO_BIG_BBOX_HEIGHT_DUCK;
		else
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
	}
	else
	{
		left = x;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::AddPointFactor()
{
	if (canMultiScoreJump || canMultiScoreLand)
		pointFactor++;
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

void CMario::Hurt()
{
	if (untouchable == 0)
	{
		if (level > MARIO_LEVEL_BIG)
		{
			SetState(MARIO_STATE_IDLE);
			level = MARIO_LEVEL_BIG;
			StartUntouchable();
		}
		else if (level > MARIO_LEVEL_SMALL)
		{
			SetState(MARIO_STATE_IDLE);
			level = MARIO_LEVEL_SMALL;
			StartUntouchable();
		}
		else
			SetState(MARIO_STATE_DIE);
	}
}

void CMario::IncreasePower()
{
	if (isOnGround && state == MARIO_STATE_PREPARE_RUN)
	{
		DWORD now = GetTickCount();
		if (power < MAX_POWER_STACK && now - run_start > 170)
		{
			power++;
			run_start = now;
		}
	}
}

void CMario::DecreasePower()
{
	DWORD now = GetTickCount();
	if (power > 0 && now - run_start > 300)
	{
		power--;
		run_start = now;
	}
}

void CMario::EatMushRoom(CMushRoom* mushroom, vector<LPGAMEOBJECT>* coObjs)
{
	switch (mushroom->type)
	{
	case MUSHROOM_TYPE_RED:
		if (level == MARIO_LEVEL_SMALL)
		{
			eat_item_start = GetTickCount();
			SetState(MARIO_STATE_EAT_ITEM);
			AddPoint(POINT_TYPE_1000);
			mushroom->Dead();
			mushroom->DeleteBehindObjs(coObjs);
			y -= MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT;
			level = MARIO_LEVEL_BIG;
		}
		break;
	case MUSHROOM_TYPE_1_UP:
		AddPoint(POINT_TYPE_1_UP);
		mushroom->Dead();
		mushroom->DeleteBehindObjs(coObjs);
		break;
	default:
		break;
	}
}

void CMario::EatLeaf(CLeaf* leaf, vector<LPGAMEOBJECT>* coObjs)
{
	eat_item_start = GetTickCount();
	SetState(MARIO_STATE_EAT_ITEM);
	AddPoint(POINT_TYPE_1000);
	leaf->Dead();
	leaf->DeleteFrontObjs(coObjs);
	if (level == MARIO_LEVEL_SMALL)
		y -= MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT;
	level = MARIO_LEVEL_RACCOON;
}

void CMario::EatCoin(CCoin* coin, vector<LPGAMEOBJECT>* coObjs)
{
	CPlayerInfo::GetInstance()->AdjustScore(POINT_50);
	CPlayerInfo::GetInstance()->AdjustMoney(MONEY_1);
	coin->Dead();
	coin->DeleteObjs(coObjs);
}


void CMario::OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs)
{
	// Fly Bar
	if (dynamic_cast<CFlyBar*>(obj))
	{
		if (obj->top != bottom && obj->bottom != top)
		{
			if (right >= obj->left && left < obj->left)
				PreventMoveX(-1, obj);
			else if (left <= obj->right && right > obj->right)
				PreventMoveX(1, obj);
		}

	}
	if (!obj->isDie && canHit)
	{
		// Para Goomba
		if (dynamic_cast<CParaGoomba*>(obj))
		{
			CParaGoomba* para = dynamic_cast<CParaGoomba*>(obj);
			if (state == MARIO_STATE_TAIL)
			{
				para->vx = nx * ENEMY_DIE_X_SPEED;
				para->vy = -ENEMY_DIE_Y_SPEED;
				para->SetState(PARA_GOOMBA_STATE_DIE_REVERSE);
				canHit = false;
			}
			else if (para->colY != -1)
				Hurt();
		}
		// Koopas
		else if (dynamic_cast<CKoopas*>(obj))
		{
			CKoopas* koopas = dynamic_cast<CKoopas*>(obj);
			if (state == MARIO_STATE_TAIL)
			{
				koopas->vx = nx * ENEMY_DIE_X_SPEED;
				if (koopas->GetState() == KOOPAS_STATE_WALKING)
					koopas->vy = -ENEMY_DIE_TAIL_Y_SPEED;
				else
					koopas->vy = -ENEMY_HIDE_TAIL_Y_SPEED;
				koopas->yReverse = true;
				koopas->SetState(KOOPAS_STATE_HIDE);
				canHit = false;
			}
			else if (state != MARIO_STATE_KICK)
			{
				if (koopas->GetState() == KOOPAS_STATE_WALKING && koopas->colY != -1)
				{
					if (untouchable == 0)
						if (koopas->vx > 0 && koopas->colX == -1 || koopas->vx < 0 && koopas->colX == 1)
							koopas->vx = -koopas->vx;
					Hurt();
				}
				else if (koopas->GetState() == KOOPAS_STATE_SPIN && koopas->colY != -1)
				{
					Hurt();
				}
			}
		}
		// Piranha
		else if (dynamic_cast<CPiranha*>(obj))
		{
			CPiranha* piranha = dynamic_cast<CPiranha*>(obj);
			if (state == MARIO_STATE_TAIL && piranha->GetState() == PIRANHA_STATE_NORMAL)
			{
				piranha->SetState(PIRANHA_STATE_DIE);
				canHit = false;
			}
			else if (piranha->GetState() == PIRANHA_STATE_NORMAL)
				Hurt();
		}
		// Goomba
		else if (dynamic_cast<CGoomba*>(obj))
		{
			CGoomba* goomba = dynamic_cast<CGoomba*>(obj);
			if (state == MARIO_STATE_TAIL)
			{
				goomba->vx = nx * ENEMY_DIE_X_SPEED;
				goomba->vy = -ENEMY_DIE_TAIL_Y_SPEED;
				goomba->SetState(GOOMBA_STATE_DIE_REVERSE);
				canHit = false;
			}
			else if (goomba->colY != -1)
				Hurt();
		}
		// Bullet Enemy
		else if (dynamic_cast<CBullet*>(obj))
		{
			CBullet* bullet = dynamic_cast<CBullet*>(obj);
			if (bullet->isEnemy)
			{
				Hurt();
			}
		}
		// MushRoom
		else if (dynamic_cast<CMushRoom*>(obj))
		{
			CMushRoom* mushroom = dynamic_cast<CMushRoom*>(obj);
			if (!mushroom->isDie)
			{
				EatMushRoom(mushroom, coObjs);
			}
		}
		// Leaf
		else if (dynamic_cast<CLeaf*>(obj))
		{
			CLeaf* leaf = dynamic_cast<CLeaf*>(obj);
			if (!leaf->isDie)
			{
				EatLeaf(leaf, coObjs);
			}
		}
		// Coin
		else if (dynamic_cast<CCoin*>(obj))
		{
			CCoin* coin = dynamic_cast<CCoin*>(obj);
			if (!coin->isDie)
			{
				EatCoin(coin, coObjs);
			}
		}
		// Question Brick
		else if (dynamic_cast<CQuestionBrick*>(obj))
		{
			CQuestionBrick* qBrick = dynamic_cast<CQuestionBrick*>(obj);
			if (state == MARIO_STATE_TAIL && isColTail(qBrick))
			{
				if (qBrick->GetState() == QUESTION_BRICK_STATE_NORMAL)
				{
					qBrick->vy = -BRICK_Y_SPEED;
					switch (qBrick->type)
					{
					case BRICK_NORMAL:
						qBrick->SetState(QUESTION_BRICK_STATE_HIT_COIN);
						break;
					case BRICK_ITEM:
						switch (level)
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
					canHit = false;
				}
			}
		}
		else if (dynamic_cast<CBreakableBrick*>(obj))
		{
			CBreakableBrick* bBrick = dynamic_cast<CBreakableBrick*>(obj);
			if (bBrick->GetState() == BREAKABLE_BRICK_STATE_COIN)
			{
				if (state != MARIO_STATE_TAIL)
				{
					CPlayerInfo::GetInstance()->AdjustScore(POINT_50);
					bBrick->DeleteObjs(coObjs);
				}
			}
			else if (bBrick->GetState() == QUESTION_BRICK_STATE_NORMAL)
			{
				if (state == MARIO_STATE_TAIL && isColTail(bBrick))
				{
					switch (bBrick->type)
					{
					case BREAKABLE_BRICK_TYPE_1UP_MUSHROOM:
						if (x <= bBrick->x)
							bBrick->SetState(BREAKABLE_BRICK_STATE_1UP_MUSHROOM_LEFT);
						else
							bBrick->SetState(BREAKABLE_BRICK_STATE_1UP_MUSHROOM_RIGHT);
						break;
					case BREAKABLE_BRICK_TYPE_ITEM:
						switch (level)
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
						bBrick->SetState(BREAKABLE_BRICK_STATE_HIT_MULTI_COIN);
						bBrick->vy = -0.1f;
						break;
					case BREAKABLE_BRICK_TYPE_P_SWITCH:
						bBrick->SetState(BREAKABLE_BRICK_STATE_P_SWITCH);
						break;
					case BREAKABLE_BRICK_TYPE_COIN:
						bBrick->SetState(BREAKABLE_BRICK_STATE_BREAK);
						break;
					default:
						break;
					}
					canHit = false;
				}
			}
		}
	}
}

bool CMario::isColTail(CGameObject* obj)
{
	float leftTail, topTail, rightTail, bottomTail;
	float left2, top2, right2, bottom2;

	topTail = top + 17;
	bottomTail = bottom - 2;
	if (nx < 0)
	{
		leftTail = left - 9;
		rightTail = leftTail + MARIO_TAIL_BBOX_WIDTH;
	}
	else
	{
		rightTail = right + 9;
		leftTail = rightTail - MARIO_TAIL_BBOX_WIDTH;
	}

	obj->GetBoundingBox(left2, top2, right2, bottom2);

	return !(rightTail < left2 || leftTail > right2 || bottomTail <= top2 || topTail >= bottom2);
}

