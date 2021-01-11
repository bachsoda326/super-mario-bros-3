#pragma once
#include "Enemy.h"
// State
#define GOOMBA_STATE_WALKING		100
#define GOOMBA_STATE_DIE			200
#define GOOMBA_STATE_DIE_REVERSE	300
// Animation
#define GOOMBA_ANI_WALKING			0
#define GOOMBA_ANI_DIE				1
// BBox
#define GOOMBA_BBOX_WIDTH			17
#define GOOMBA_BBOX_HEIGHT			16
#define GOOMBA_BBOX_HEIGHT_DIE		9
// Speed
#define GOOMBA_WALKING_SPEED		0.035f
// Time
#define GOOMBA_DIE_TIME				300

class CGoomba : public CEnemy
{	
	void SetBoundingBox(); 
public:
	DWORD die_start = 0;

	CGoomba();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};