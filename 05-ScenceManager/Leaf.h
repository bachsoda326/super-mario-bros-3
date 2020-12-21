#pragma once
#include "GameObject.h"

#define LEAF_BBOX_WIDTH 17
#define LEAF_BBOX_HEIGHT 14
#define LEAF_BBOX_TOP_HIGH 48

#define LEAF_ANI_SET 25

class CLeaf : public CGameObject
{	
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);

public:
	DWORD stopTime_start = 0;
	bool isStop = false;
	bool isInitialized = false;
	float start_top;
	float start_x;

	CLeaf();
	CLeaf(float x, float y);
};

