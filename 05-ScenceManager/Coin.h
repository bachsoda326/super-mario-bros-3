#pragma once
#include "GameObject.h"

#define COIN_BBOX_WIDTH 16
#define COIN_BBOX_HEIGHT 16
#define COIN_ANI_SET 25

#define COIN_GRAVITY 0.001f

class CCoin : public CGameObject
{
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);

public:
	float start_x;			// initial position of coin
	float start_y;

	CCoin();
	CCoin(float x, float y, bool isInBrick = false);
	bool isInBrick = false;
};

