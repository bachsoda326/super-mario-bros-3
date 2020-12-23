#pragma once
#include "Brick.h"

#define BREAKABLE_BRICK_STATE_NORMAL				100
#define BREAKABLE_BRICK_STATE_BREAK					200
#define BREAKABLE_BRICK_STATE_COIN					300
#define BREAKABLE_BRICK_STATE_1UP_MUSHROOM_LEFT		400
#define BREAKABLE_BRICK_STATE_1UP_MUSHROOM_RIGHT	500
#define BREAKABLE_BRICK_STATE_P_SWITCH				600

#define BREAKABLE_BRICK_ANI_NORMAL	0
#define BREAKABLE_BRICK_ANI_HIT		1
#define BREAKABLE_BRICK_ANI_COIN	2

#define BREAKABLE_BRICK_TYPE_COIN			1
#define BREAKABLE_BRICK_TYPE_1UP_MUSHROOM	2
#define BREAKABLE_BRICK_TYPE_P_SWITCH		3

class CBreakableBrick : public CBrick
{
	DWORD coin_start;
	DWORD break_start;

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);

public:
	int type;

	CGameObject* obj = NULL;

	CBreakableBrick(int type);
	
	void SetState(int state);
};

