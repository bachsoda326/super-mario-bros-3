#pragma once
#include "GameObject.h"
#include "Bullet.h"
// State
#define PIRANHA_STATE_NORMAL			100
#define PIRANHA_STATE_HIDE				200
#define PIRANHA_STATE_DIE				300
// Animation
#define PIRANHA_ANI_FIRE_RED_DOWN		0
#define PIRANHA_ANI_FIRE_RED_UP			1
#define PIRANHA_ANI_NOR					2
#define PIRANHA_ANI_FIRE_DOWN			3
#define PIRANHA_ANI_FIRE_UP				4
#define PIRANHA_ANI_DIE					5
// BBox
#define PIRANHA_BBOX_WIDTH				17
#define PIRANHA_FIRE_RED_BBOX_HEIGHT	32
#define PIRANHA_NOR_BBOX_HEIGHT			24
#define PIRANHA_FIRE_BBOX_HEIGHT		25
#define PIRANHA_BBOX_HEAD_HEIGHT		5
// Type
#define PIRANHA_NOR						0
#define PIRANHA_FIRE					1
#define PIRANHA_FIRE_RED				2
// Speed
#define PIRANHA_Y_SPEED					0.025f
// Time
#define PIRANHA_EXPLODE_TIME			300
#define PIRANHA_FIRE_UP_TIME			1500
#define PIRANHA_NOR_UP_TIME				750
#define PIRANHA_FIRE_DOWN_TIME			2500
#define PIRANHA_NOR_DOWN_TIME			1500
#define PIRANHA_WAIT_FIRE_TIME			1000
// Others
#define PIRANHA_ANI_SET					27
#define PIRANHA_BOTTOM_DISTANCE			8
#define PIRANHA_BOTTOM_DISTANCE_1		5
#define PIRANHA_MIN_DISTANCE_X			25
#define PIRANHA_MAX_DISTANCE_X			135
#define PIRANHA_FAR_DISTANCE_X			75


class CPiranha : public CGameObject
{
	DWORD wait_up_start = 0;
	DWORD wait_down_start = 0;
	DWORD wait_fire_start = 0;
	DWORD explode_start = 0;
	bool isUp = false;

	//CBullet* bullet = NULL;

	void SetBoundingBox();
public:
	int type;
	float start_top;
	float start_bottom;
	float height;

	CPiranha(int type, float x, float y);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);
	void Render();
	void SetState(int state);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

