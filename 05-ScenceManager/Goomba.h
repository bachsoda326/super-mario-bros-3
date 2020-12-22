#pragma once
#include "GameObject.h"
#include "Box.h"
#include "Ground.h"
#include "WarpPipe.h"
#include "Koopas.h"
#include "Brick.h"

#define GOOMBA_WALKING_SPEED 0.05f;

#define GOOMBA_BBOX_WIDTH 17
#define GOOMBA_BBOX_HEIGHT 16
#define GOOMBA_BBOX_HEIGHT_DIE 9

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_DIE_REVERSE 300

#define GOOMBA_ANI_WALKING 0
#define GOOMBA_ANI_DIE 1

#define GOOMBA_GRAVITY			0.001f

class CGoomba : public CGameObject
{
	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();

public: 	
	DWORD die_start;

	CGoomba();
	void SetState(int state);
};