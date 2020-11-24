#pragma once
#include "GameObject.h"

#define BULLET_GRAVITY	0.001f
#define BULLET_VX_SPEED 0.2f
#define BULLET_VY_SPEED 0.02f

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
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	DWORD explode_start;

	CBullet();
	virtual void SetState(int state);
};
