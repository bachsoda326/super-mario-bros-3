#pragma once
#include "Enemy.h"
#include "Box.h"
#include "Ground.h"
#include "WarpPipe.h"
#include "Brick.h"
#include "Goomba.h"
// State
#define KOOPAS_STATE_WALKING			100
#define KOOPAS_STATE_HIDE				200
#define KOOPAS_STATE_SPIN				300
#define KOOPAS_STATE_HOLD				400
#define KOOPAS_STATE_SHAKE				500
#define KOOPAS_STATE_DIE				999
// Animation
#define KOOPAS_ANI_WALKING_LEFT			0
#define KOOPAS_ANI_WALKING_RIGHT		1
#define KOOPAS_ANI_SPIN_LEFT			2
#define KOOPAS_ANI_SPIN_RIGHT			3
#define KOOPAS_ANI_HIDE					4
#define KOOPAS_ANI_SHAKE				5
#define KOOPAS_ANI_WING					6
// BBox
#define KOOPAS_BBOX_WIDTH				16
#define KOOPAS_BBOX_HEIGHT				27
#define KOOPAS_BBOX_HEIGHT_HIDE			15
// Speed
#define KOOPAS_GRAVITY					0.0007f
#define KOOPAS_WING_GRAVITY				0.0005f
#define KOOPAS_WALKING_SPEED			0.035f
#define KOOPAS_WALKING_WING_SPEED		0.02f
#define KOOPAS_SPIN_SPEED				0.215f
#define KOOPAS_WING_JUMP				0.175f
#define KOOPAS_WING_Y_SPEED				0.05f
#define KOOPAS_DIE_X_SPEED				0.07f
#define KOOPAS_DIE_Y_SPEED				0.2f
#define KOOPAS_HIT_Y_SPEED				0.1f
// Type
#define KOOPAS_GREEN					1
#define KOOPAS_RED						2
#define KOOPAS_GREEN_WING				3
#define KOOPAS_RED_WING					4
#define KOOPAS_HIDE						5
// Time
#define KOOPAS_SHAKE_TIME				1000
#define KOOPAS_HIDE_TIME				5000
#define KOOPAS_HIDE_HOLD_TIME			8000
// Others
#define KOOPAS_HOLD_DISTANCE_X			3
#define KOOPAS_HOLD_DISTANCE_Y_SMALL	4
#define KOOPAS_HOLD_DISTANCE_Y_BIG		9

class CKoopas : public CEnemy
{
	DWORD hide_start = 0;
	DWORD shake_start = 0;

	bool isShaking = false;

	float yMin = -1, yMax = -1;

	void SetBoundingBox();
public:
	int type;

	CKoopas(int type);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetFlyRegion(float min, float max);

	void UpdateDirection();

	// Intersect with objs
	void OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs);
};