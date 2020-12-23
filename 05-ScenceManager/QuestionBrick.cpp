#include "QuestionBrick.h"
#include "Coin.h"
#include "MushRoom.h"
#include "Leaf.h"
#include "Game.h"

CQuestionBrick::CQuestionBrick(float x, float y, int type)
{
	this->type = type;
	start_x = x;
	start_y = y;
	SetState(QUESTION_BRICK_STATE_NORMAL);
}

void CQuestionBrick::Render()
{
	/*if (obj != NULL)
	{
		if (!obj->isDead)
			obj->Render();
	}*/

	int ani = -1;
	if (state == QUESTION_BRICK_STATE_NORMAL)
		ani = QUESTION_BRICK_ANI_NORMAL;
	else
		ani = QUESTION_BRICK_ANI_HIT;
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();	
}

void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	/*if (obj != NULL)
	{
		if (!obj->isDead)
			obj->Update(dt, coObjects);
		else
			obj = NULL;
	}*/

	if (state != QUESTION_BRICK_STATE_NORMAL)
	{
		CGameObject::Update(dt, coObjects);

		x += dx;
		y += dy;

		if (y != start_y)
			vy += QUESTION_BRICK_GRAVITY * dt;

		if (y >= start_y)
		{
			y = start_y;
			vy = 0;			

			if (state != QUESTION_BRICK_STATE_HIT_COIN)
			{
				switch (state)
				{
				case QUESTION_BRICK_STATE_HIT_MUSHROOM_LEFT:
					obj = new CMushRoom(start_x, start_y, -1, MUSHROOM_TYPE_1_UP);
					CGame::GetInstance()->GetCurrentScene()->GetOtherObjs()->push_back(obj);
					break;
				case QUESTION_BRICK_STATE_HIT_MUSHROOM_RIGHT:
					obj = new CMushRoom(start_x, start_y, 1, MUSHROOM_TYPE_1_UP);
					CGame::GetInstance()->GetCurrentScene()->GetOtherObjs()->push_back(obj);
					break;
				default:
					break;
				}
			}

			SetState(QUESTION_BRICK_STATE_STATIC);
		}
	}
}

void CQuestionBrick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case QUESTION_BRICK_STATE_HIT_COIN:
		obj = new CCoin(start_x, start_y - COIN_BBOX_HEIGHT, true);
		CGame::GetInstance()->GetCurrentScene()->GetOtherObjs()->push_back(obj);
		//obj->vy = -0.5f;
		break;
		/*case QUESTION_BRICK_STATE_HIT_MUSHROOM_LEFT:
			if (isCreateItem)
				obj = new CMushRoom(start_x, start_y, -1);
			break;
		case QUESTION_BRICK_STATE_HIT_MUSHROOM_RIGHT:
			if (isCreateItem)
				obj = new CMushRoom(start_x, start_y, 1);
			break;*/
	case QUESTION_BRICK_STATE_HIT_LEAF:
		obj = new CLeaf(x, y);
		CGame::GetInstance()->GetCurrentScene()->GetObjs()->push_back(obj);
		break;
	default:
		break;
	}
}
