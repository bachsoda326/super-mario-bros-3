#include "QuestionBrick.h"
#include "Coin.h"
#include "MushRoom.h"
#include "Leaf.h"

CQuestionBrick::CQuestionBrick(float x, float y, int type)
{
	this->type = type;
	start_x = x;
	start_y = y;
	SetState(QUESTION_BRICK_STATE_NORMAL);
}

void CQuestionBrick::Render()
{
	int ani = -1;
	if (state == QUESTION_BRICK_STATE_NORMAL)
		ani = 1;
	else
		ani = 0;
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();

	if (obj != NULL)
	{
		if (!obj->isDead)
			obj->Render();
	}
}

void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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
	}

	if (obj != NULL)
	{
		if (!obj->isDead)
			obj->Update(dt, coObjects);
		else
			obj = NULL;
	}
}

void CQuestionBrick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case QUESTION_BRICK_STATE_HIT_COIN:
		obj = new CCoin(start_x, start_y - COIN_BBOX_HEIGHT, true);
		//obj->vy = -0.5f;
		break;
	case QUESTION_BRICK_STATE_HIT_MUSHROOM:
		obj = new CMushRoom();
		break;
	case QUESTION_BRICK_STATE_HIT_LEAF:
		obj = new CLeaf();
		break;
	default:
		break;
	}
}
