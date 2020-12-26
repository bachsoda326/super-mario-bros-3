#pragma once
#include "GameObject.h"

class CGround : public CGameObject
{
	int mRight, mBottom;

	void SetBoundingBox();
public:
	CGround(int r, int b);

	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

