#pragma once

#include "GameObject.h"

#define KOOPAS_WALKING_SPEED 0.03f;

#define KOOPAS_BBOX_WIDTH 17
#define KOOPAS_BBOX_HEIGHT 27
#define KOOPAS_BBOX_HEIGHT_DIE 17

#define KOOPAS_STATE_WALKING 100
#define KOOPAS_STATE_DIE 200

#define KOOPAS_ANI_WALKING_LEFT 0
#define KOOPAS_ANI_WALKING_RIGHT 1
#define KOOPAS_ANI_SPIN_LEFT 2
#define KOOPAS_ANI_SPIN_RIGHT 3
#define KOOPAS_ANI_DIE 4

class CKoopas : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

public:
	CKoopas();
	virtual void SetState(int state);
};