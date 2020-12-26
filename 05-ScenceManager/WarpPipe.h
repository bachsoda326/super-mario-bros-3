#pragma once
#include "GameObject.h"
#include "Piranha.h"

#define WARPPIPE_TYPE_DOWN	3
#define WARPPIPE_TYPE_UP	4

class CWarpPipe : public CGameObject
{
	int mRight, mBottom;
	CPiranha* piranha = NULL;

	void SetBoundingBox();
public:
	int type;
	float tele_x = -1, tele_y = -1;

	CWarpPipe(float x, float y, int r, int b, int type, float tele_x, float tele_y);

	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

