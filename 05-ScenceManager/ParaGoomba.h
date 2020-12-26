#pragma once
#include "GameObject.h"

#define PARA_GOOMBA_WALKING_SPEED 0.035f
		
#define PARA_GOOMBA_WING_BBOX_WIDTH 20
#define PARA_GOOMBA_WING_BBOX_HEIGHT 23
#define PARA_GOOMBA_BBOX_WIDTH 16
#define PARA_GOOMBA_BBOX_HEIGHT 16
#define PARA_GOOMBA_BBOX_HEIGHT_DIE 10
		
#define PARA_GOOMBA_STATE_WALKING		100
#define PARA_GOOMBA_STATE_PREPARE_JUMP	200
#define PARA_GOOMBA_STATE_JUMP			300
#define PARA_GOOMBA_STATE_LOOSE_WING	400
#define PARA_GOOMBA_STATE_DIE			500
#define PARA_GOOMBA_STATE_DIE_REVERSE	600
		
#define PARA_GOOMBA_ANI_WING_WALKING		0
#define PARA_GOOMBA_ANI_WING_PREPARE_JUMP	1
#define PARA_GOOMBA_ANI_WING_JUMP			2
#define PARA_GOOMBA_ANI_WALKING				3
#define PARA_GOOMBA_ANI_DIE					4
		
#define PARA_GOOMBA_GRAVITY			0.0005f
#define PARA_GOOMBA_DIE_GRAVITY		0.001f

#define PARA_GOOMBA_LEVEL_NOR		1
#define PARA_GOOMBA_LEVEL_WING		2


class CParaGoomba : public CGameObject
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

