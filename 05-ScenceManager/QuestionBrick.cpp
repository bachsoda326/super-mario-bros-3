#include "QuestionBrick.h"

CQuestionBrick::CQuestionBrick(float x, float y)
{
	start_x = x;
	start_y = y;
}

void CQuestionBrick::Render()
{
	int ani = -1;
	if (!isColision)
		ani = 1;
	else
		ani = 0;
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
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
}
