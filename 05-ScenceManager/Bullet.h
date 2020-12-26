#pragma once
#include "GameObject.h"

#define BULLET_ANI_SET	2
#define BULLET_GRAVITY	0.001f
#define BULLET_VX_SPEED 0.2f
#define BULLET_VY_SPEED 0.02f
#define BULLET_ENEMY_VX_SPEED 0.045f
#define BULLET_ENEMY_NEAR_VY_SPEED 0.045f
#define BULLET_ENEMY_FAR_VY_SPEED 0.0175f

#define BULLET_BBOX_WIDTH 8
#define BULLET_BBOX_HEIGHT 8

#define BULLET_STATE_FIRE 100
#define BULLET_STATE_EXPLODE 200
#define BULLET_STATE_DISAPPEAR 999

#define BULLET_ANI_FIRE_LEFT 152
#define BULLET_ANI_FIRE_RIGHT 153
#define BULLET_ANI_EXPLODE 154


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

