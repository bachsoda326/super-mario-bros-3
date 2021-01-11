#pragma once
#include "Enemy.h"
// State
#define PARA_GOOMBA_STATE_WALKING			100
#define PARA_GOOMBA_STATE_PREPARE_JUMP		200
#define PARA_GOOMBA_STATE_JUMP				300
#define PARA_GOOMBA_STATE_LOOSE_WING		400
#define PARA_GOOMBA_STATE_DIE				500
#define PARA_GOOMBA_STATE_DIE_REVERSE		600
// Animation
#define PARA_GOOMBA_ANI_WING_WALKING		0
#define PARA_GOOMBA_ANI_WING_PREPARE_JUMP	1
#define PARA_GOOMBA_ANI_WING_JUMP			2
#define PARA_GOOMBA_ANI_WALKING				3
#define PARA_GOOMBA_ANI_DIE					4
#define PARA_GOOMBA_WALKING_SPEED			0.035f
// BBox
#define PARA_GOOMBA_WING_BBOX_WIDTH			20
#define PARA_GOOMBA_WING_BBOX_HEIGHT		23
#define PARA_GOOMBA_BBOX_WIDTH				16
#define PARA_GOOMBA_BBOX_HEIGHT				16
#define PARA_GOOMBA_BBOX_HEIGHT_DIE			10
// Speed		
#define PARA_GOOMBA_GRAVITY					0.0005f
#define PARA_GOOMBA_PREPARE_JUMP_SPEED		0.07f
#define PARA_GOOMBA_JUMP_SPEED				0.18f
// Level
#define PARA_GOOMBA_LEVEL_NOR				1
#define PARA_GOOMBA_LEVEL_WING				2
// Time
#define PARA_GOOMBA_WALK_TIME				800
#define PARA_GOOMBA_PREPARE_JUMP_TIME		600
// Others
#define PARA_GOOMBA_DISTANCE_MIN_X			30
#define PARA_GOOMBA_DISTANCE_MAX_X			50

class CParaGoomba : public CEnemy
{	
	DWORD wing_walk_start = 0;
	DWORD wing_pre_jump_start = 0; 
	DWORD die_start = 0;

	int level;	

	void SetBoundingBox();
public:
	CParaGoomba();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetLevel() { return level; }
};

