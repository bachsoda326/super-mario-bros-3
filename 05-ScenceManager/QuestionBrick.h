#pragma once
#include "Brick.h"

#define QUESTION_BRICK_GRAVITY 0.001f

#define QUESTION_BRICK_STATE_NORMAL				100
#define QUESTION_BRICK_STATE_HIT_COIN			200
#define QUESTION_BRICK_STATE_HIT_MUSHROOM		300
#define QUESTION_BRICK_STATE_HIT_LEAF			400
#define QUESTION_BRICK_STATE_HIT_FIRE			500
#define QUESTION_BRICK_STATE_HIT_GREEN_MUSHROOM 600
#define QUESTION_BRICK_STATE_HIT_STAR			600

#define QUESTION_BRICK_TYPE_COIN	1
#define QUESTION_BRICK_TYPE_OBJECT	2

class CQuestionBrick : public CBrick
{
public:
	float start_x;			// initial position of question brick
	float start_y;

	int type;
	CGameObject *obj = NULL;

	CQuestionBrick(float x, float y, int type);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void SetState(int state);
};

