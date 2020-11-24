#pragma once

#include "GameObject.h"
#include "Box.h"
#include "Ground.h"
#include "WarpPipe.h"
#include "Brick.h"
#include "Goomba.h"

#define KOOPAS_WALKING_SPEED 0.05f;
#define KOOPAS_SPIN_SPEED 0.25f;

#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 27
#define KOOPAS_BBOX_HEIGHT_HIDE 17

#define KOOPAS_STATE_WALKING 100
#define KOOPAS_STATE_HIDE 200
#define KOOPAS_STATE_SPIN 300
#define KOOPAS_STATE_HOLD 400
#define KOOPAS_STATE_DIE 500

#define KOOPAS_ANI_WALKING_LEFT 0
#define KOOPAS_ANI_WALKING_RIGHT 1
#define KOOPAS_ANI_SPIN_LEFT 2
#define KOOPAS_ANI_SPIN_RIGHT 3
#define KOOPAS_ANI_HIDE 4

#define KOOPAS_GRAVITY			0.002f

#define KOOPAS_GREEN 1
#define KOOPAS_RED 2

class CKoopas : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

public:
	int type;
	/*float xMin, xMax;*/

	CKoopas(int type);
	virtual void SetState(int state);
};