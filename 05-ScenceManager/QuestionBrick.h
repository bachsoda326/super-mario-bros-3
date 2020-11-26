#pragma once
#include "Brick.h"

#define QUESTION_BRICK_GRAVITY 0.001f

class CQuestionBrick : public CBrick
{
public:
	float start_x;			// initial position of question brick
	float start_y;

	bool isColision = false;
	CQuestionBrick(float x, float y);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
};

