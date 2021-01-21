#pragma once
#include "GameObject.h"

#define WORLD_HAMMER_X_SPEED	0.01f

class CWorldHammer : public CGameObject
{
	float x_left, x_right;

	void SetBoundingBox() {};
public:
	CWorldHammer(float left, float right);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
};

