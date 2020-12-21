#pragma once
#include "GameObject.h"
#include "Piranha.h"

class CWarpPipe : public CGameObject
{
	int mRight, mBottom;
	CPiranha* piranha = NULL;

public:
	int type;

	CWarpPipe(float x, float y, int r, int b, int type);

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

