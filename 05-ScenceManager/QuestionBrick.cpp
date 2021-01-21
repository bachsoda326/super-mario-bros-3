#include "QuestionBrick.h"
#include "Coin.h"
#include "MushRoom.h"
#include "Leaf.h"
#include "Game.h"
#include "PlayerInfo.h"

CQuestionBrick::CQuestionBrick(float x, float y, int type)
{
	this->type = type;
	start_x = x;
	start_y = y;
	SetState(QUESTION_BRICK_STATE_NORMAL);
}

void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CBrick::Update(dt, coObjects);

	// When jumped hit by Mario
	if (state != QUESTION_BRICK_STATE_NORMAL)
	{
		CGameObject::Update(dt, coObjects);

		x += dx;
		y += dy;

		if (y != start_y)
			vy += QUESTION_BRICK_GRAVITY * dt;

		if (vy < 0)
			isUp = true;
		else
			isUp = false;

		if (y >= start_y)
		{
			y = start_y;
			vy = 0;			

			// Init mushroom
			if (state != QUESTION_BRICK_STATE_HIT_COIN)
			{
				switch (state)
				{
				case QUESTION_BRICK_STATE_HIT_MUSHROOM_LEFT:
					obj = new CMushRoom(start_x, start_y, MUSHROOM_LEFT, MUSHROOM_TYPE_RED);
					CGame::GetInstance()->GetCurrentScene()->GetBehindObjs()->push_back(obj);
					break;
				case QUESTION_BRICK_STATE_HIT_MUSHROOM_RIGHT:
					obj = new CMushRoom(start_x, start_y, MUSHROOM_RIGHT, MUSHROOM_TYPE_RED);
					CGame::GetInstance()->GetCurrentScene()->GetBehindObjs()->push_back(obj);
					break;
				default:
					break;
				}
			}

			SetState(QUESTION_BRICK_STATE_STATIC);
		}
	}
}

void CQuestionBrick::Render()
{
	int ani = QUESTION_BRICK_ANI_NORMAL;
	if (state == QUESTION_BRICK_STATE_NORMAL)
		ani = QUESTION_BRICK_ANI_NORMAL;
	else
		ani = QUESTION_BRICK_ANI_HIT;
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();	
}

void CQuestionBrick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case QUESTION_BRICK_STATE_HIT_COIN:
		obj = new CCoin(start_x, start_y - COIN_BBOX_HEIGHT, true);
		CGame::GetInstance()->GetCurrentScene()->GetBehindObjs()->push_back(obj);
		CPlayerInfo::GetInstance()->AdjustMoney(MONEY_1);		
		break;		
	case QUESTION_BRICK_STATE_HIT_LEAF:
		obj = new CLeaf(x, y);
		CGame::GetInstance()->GetCurrentScene()->GetFrontObjs()->push_back(obj);
		break;
	default:
		break;
	}
}
