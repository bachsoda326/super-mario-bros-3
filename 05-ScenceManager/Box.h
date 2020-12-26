#pragma once
#include "GameObject.h"

class CBox : public CGameObject
{
	int mRight, mBottom;

	void SetBoundingBox();
public:
	CBox(int r, int b);

	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

