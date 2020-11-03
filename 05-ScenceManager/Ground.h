#pragma once
#include "GameObject.h"

class CGround : public CGameObject
{
	int right, bottom;
public:
	CGround(int r, int b);

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

