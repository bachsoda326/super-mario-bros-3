#pragma once
#include "Brick.h"
// State
#define BREAKABLE_BRICK_STATE_NORMAL				100
#define BREAKABLE_BRICK_STATE_BREAK					200
#define BREAKABLE_BRICK_STATE_COIN					300
#define BREAKABLE_BRICK_STATE_1UP_MUSHROOM_LEFT		400
#define BREAKABLE_BRICK_STATE_1UP_MUSHROOM_RIGHT	401
#define BREAKABLE_BRICK_STATE_HIT_MUSHROOM_LEFT		500
#define BREAKABLE_BRICK_STATE_HIT_MUSHROOM_RIGHT	501
#define BREAKABLE_BRICK_STATE_HIT_LEAF				600
#define BREAKABLE_BRICK_STATE_P_SWITCH				700
#define BREAKABLE_BRICK_STATE_HIT_MULTI_COIN		800
#define BREAKABLE_BRICK_STATE_HIT					900
// Animation
#define BREAKABLE_BRICK_ANI_NORMAL					0
#define BREAKABLE_BRICK_ANI_HIT						1
#define BREAKABLE_BRICK_ANI_COIN					2
// Type
#define BREAKABLE_BRICK_TYPE_COIN					1
#define BREAKABLE_BRICK_TYPE_1UP_MUSHROOM			2
#define BREAKABLE_BRICK_TYPE_P_SWITCH				3
#define BREAKABLE_BRICK_TYPE_ITEM					4
#define BREAKABLE_BRICK_TYPE_MULTI_COIN				5
// Speed
#define BREAKABLE_BRICK_GRAVITY						0.001f
// Time
#define BREAKABLE_BRICK_TIME						5000
// Others
#define BREAKABLE_BRICK_NUM_BRICK_PIECES			4

class CBreakableBrick : public CBrick
{
	DWORD coin_start;
	DWORD break_start;

	int coinCount = 10;		// For multi coin type

	void SetBoundingBox();
public:
	int type;

	CGameObject* obj = NULL;

	CBreakableBrick(int type);

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
};

