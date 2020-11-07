#pragma once
#include "GameObject.h"

#define LEAF_BBOX_WIDTH 17
#define LEAF_BBOX_HEIGHT 14

class CLeaf : public CGameObject
{	
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

