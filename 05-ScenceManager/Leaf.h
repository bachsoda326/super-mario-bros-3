#pragma once
#include "GameObject.h"
// Animation
#define LEAF_ANI_SET			25
// BBox
#define LEAF_BBOX_WIDTH			17
#define LEAF_BBOX_HEIGHT		14
#define LEAF_BBOX_TOP_HIGH		48
// Speed
#define LEAF_X_SPEED			0.06f
#define LEAF_Y_SPEED			0.05f
#define LEAF_Y_UP_SPEED			0.008f
// Time
#define LEAF_STOP_TIME			50
// Others
#define LEAF_DISTANCE_X			30
#define LEAF_DISTANCE_X_1		20
#define LEAF_DISTANCE_X_2		26

class CLeaf : public CGameObject
{	
	void SetBoundingBox();
public:
	DWORD stopTime_start = 0;
	bool isStop = false;
	bool isInitialized = false;
	float start_top = -1;
	float start_x = -1;

	CLeaf();
	CLeaf(float x, float y);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

