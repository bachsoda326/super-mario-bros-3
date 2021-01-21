#pragma once
#include "GameObject.h"

#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16
#define MUSHROOM_ANI_SET 25

#define MUSHROOM_TYPE_RED	1
#define MUSHROOM_TYPE_1_UP	2

#define MUSHROOM_ANI_RED	1
#define MUSHROOM_ANI_1_UP	3

#define MUSHROOM_GRAVITY			0.001f

#define MUSHROOM_LEFT		-1
#define MUSHROOM_RIGHT		1

class CMushRoom : public CGameObject
{	
	void SetBoundingBox();
public:
	int type = -1;
	bool isInitialized = false;
	int nx = -1;
	float start_bottom = -1;

	CMushRoom();
	CMushRoom(float x, float y, int nx, int type);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

