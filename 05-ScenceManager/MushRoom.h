#pragma once
#include "GameObject.h"

#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16
#define MUSHROOM_ANI_SET 25

#define MUSHROOM_GRAVITY			0.001f

class CMushRoom : public CGameObject
{
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);

public:
	bool isInitialized = false;
	int nx;
	float start_bottom;
	CMushRoom();
	CMushRoom(float x, float y, int nx);
};

