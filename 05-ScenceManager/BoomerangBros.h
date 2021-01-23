#pragma once
#include "Enemy.h"
// State
#define BOOMERANGBROS_STATE_WALKING		1
#define BOOMERANGBROS_STATE_THROW		2
#define BOOMERANGBROS_STATE_DIE			3
// State
#define BOOMERANGBROS_ANI_WALKING		0
#define BOOMERANGBROS_ANI_THROW			1
// BBox
#define BOOMERANGBROS_BBOX_WIDTH		16
#define BOOMERANGBROS_BBOX_HEIGHT		24
// Speed
#define BOOMERANGBROS_WALKING_SPEED		0.035f
// Time
#define BOOMERANGBROS_WAIT_TIME			3500
#define BOOMERANGBROS_THROW_TIME		2000
// Others
#define BOOMERANGBROS_DISTANCE_X		20

class CBoomerangBros : public CEnemy
{
	float start_x = -1;

	DWORD wait_time = 0;
	DWORD throw_time = 0;

	void SetBoundingBox();

public:
	CBoomerangBros(float x);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);

	void ThrowBoomerang();
};

