#pragma once
#include "GameObject.h"

class CWorldHammer : public CGameObject
{
	float x_left, x_right;

	void SetBoundingBox() {};
public:
	CWorldHammer(float left, float right);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
};

