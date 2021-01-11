#pragma once
#include "GameObject.h"
// State
#define BULLET_STATE_FIRE			100
#define BULLET_STATE_EXPLODE		200
#define BULLET_STATE_DISAPPEAR		999
// Animation
#define BULLET_ANI_FIRE_LEFT		152
#define BULLET_ANI_FIRE_RIGHT		153
#define BULLET_ANI_EXPLODE			154
#define BULLET_ANI_SET				2
// BBox
#define BULLET_BBOX_WIDTH			8
#define BULLET_BBOX_HEIGHT			8
// Speed
#define BULLET_GRAVITY				0.001f
#define BULLET_X_SPEED				0.2f
#define BULLET_Y_SPEED				0.02f
#define BULLET_ENEMY_X_SPEED		0.045f
#define BULLET_ENEMY_NEAR_Y_SPEED	0.045f
#define BULLET_ENEMY_FAR_Y_SPEED	0.0175f
#define BULLET_BOUND_Y_SPEED		0.2f
// Time
#define BULLET_EXPLODE_TIME			300


class CBullet : public CGameObject
{
	void SetBoundingBox();
public:
	bool isEnemy = false;
	DWORD explode_start = 0;

	CBullet();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	// Giao nhau vs các obj
	void OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs);
};

