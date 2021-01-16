#pragma once
#include "GameObject.h"

// State
#define FLY_BAR_STATE_SPAWN			100
#define FLY_BAR_STATE_FALL			200
// BBox
#define FLY_BAR_BBOX_WIDTH			48
#define FLY_BAR_BBOX_HEIGHT			16
// Speed
#define FLY_BAR_GRAVITY				0.0002f
#define FLY_BAR_X_SPEED				0.05f

class CFlyBar : public CGameObject
{
	void SetBoundingBox();

public:
	CFlyBar();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
};

