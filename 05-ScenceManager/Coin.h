#pragma once
#include "GameObject.h"

#define COIN_BBOX_WIDTH 15
#define COIN_BBOX_HEIGHT 16
#define COIN_ANI_SET 25

#define COIN_GRAVITY 0.002f

class CCoin : public CGameObject
{
	void SetBoundingBox();
public:
	float start_x = -1;			// initial position of coin
	float start_y = -1;
	bool isInBrick = false;

	CCoin();
	CCoin(float x, float y, bool isInBrick = false);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

