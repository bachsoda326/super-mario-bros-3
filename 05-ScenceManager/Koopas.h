#pragma once

#include "GameObject.h"
#include "Box.h"
#include "Ground.h"
#include "WarpPipe.h"
#include "Brick.h"
#include "Goomba.h"

#define KOOPAS_WALKING_SPEED 0.035f
#define KOOPAS_WALKING_WING_SPEED 0.02f
#define KOOPAS_SPIN_SPEED 0.215f

#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 27
#define KOOPAS_BBOX_HEIGHT_HIDE 15

#define KOOPAS_STATE_WALKING	100
#define KOOPAS_STATE_HIDE		200
#define KOOPAS_STATE_SPIN		300
#define KOOPAS_STATE_HOLD		400
#define KOOPAS_STATE_SHAKE		500
#define KOOPAS_STATE_DIE		999

#define KOOPAS_ANI_WALKING_LEFT		0
#define KOOPAS_ANI_WALKING_RIGHT	1
#define KOOPAS_ANI_SPIN_LEFT		2
#define KOOPAS_ANI_SPIN_RIGHT		3
#define KOOPAS_ANI_HIDE				4
#define KOOPAS_ANI_SHAKE			5
#define KOOPAS_ANI_WING				6

#define KOOPAS_GRAVITY			0.002f
#define KOOPAS_WING_GRAVITY		0.0005f
#define KOOPAS_WING_JUMP		0.175f

#define KOOPAS_GREEN		1
#define KOOPAS_RED			2
#define KOOPAS_GREEN_WING	3

class CKoopas : public CGameObject
{
	DWORD hide_start;
	DWORD shake_start;

	bool isShaking = false;

	void SetBoundingBox();
public:
	int type;
	/*float xMin, xMax;*/

	CKoopas(int type);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	// Giao nhau vs các obj
	void OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs);
};