#pragma once
#include "GameObject.h"

class CBox : public CGameObject
{
	int right, bottom;
public:
	CBox(int r, int b);

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

