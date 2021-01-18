#pragma once
#include "Brick.h"

#define QUESTION_BRICK_GRAVITY 0.001f

#define QUESTION_BRICK_STATE_NORMAL				100
#define QUESTION_BRICK_STATE_HIT_COIN			200
#define QUESTION_BRICK_STATE_HIT_MUSHROOM_LEFT	300
#define QUESTION_BRICK_STATE_HIT_MUSHROOM_RIGHT	400
#define QUESTION_BRICK_STATE_HIT_LEAF			500
#define QUESTION_BRICK_STATE_HIT_FIRE			600
#define QUESTION_BRICK_STATE_HIT_GREEN_MUSHROOM 700
#define QUESTION_BRICK_STATE_HIT_STAR			800
#define QUESTION_BRICK_STATE_STATIC				999

#define QUESTION_BRICK_TYPE_COIN	1
#define QUESTION_BRICK_TYPE_OBJECT	2

#define QUESTION_BRICK_ANI_NORMAL	0
#define QUESTION_BRICK_ANI_HIT		1

class CQuestionBrick : public CBrick
{
public:
	int type;
	bool isCreateItem = false;
	CGameObject *obj = NULL;

	CQuestionBrick(float x, float y, int type);	

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void SetState(int state);
};

