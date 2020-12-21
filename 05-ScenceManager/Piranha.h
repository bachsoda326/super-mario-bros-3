#pragma once
#include "GameObject.h"
#include "Bullet.h"

#define PIRANHA_NOR			0
#define PIRANHA_FIRE		1
#define PIRANHA_FIRE_RED	2
#define PIRANHA_ANI_SET		27

#define PIRANHA_BBOX_WIDTH				17
#define PIRANHA_FIRE_RED_BBOX_HEIGHT	32
#define PIRANHA_NOR_BBOX_HEIGHT			24
#define PIRANHA_FIRE_BBOX_HEIGHT		25

class CPiranha : public CGameObject
{
	DWORD wait_up_start;
	DWORD wait_down_start;

	CBullet* bullet = NULL;

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);

public:
	int type;
	float start_top;
	float start_bottom;

	CPiranha(int type, float x, float y);
};

