#pragma once
#include "GameObject.h"

class CWarpPipe : public CGameObject
{
	int right, bottom;
public:
	CWarpPipe(int r, int b);

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

