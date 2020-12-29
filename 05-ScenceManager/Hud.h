#pragma once
#include "GameObject.h"

class Hud : public CGameObject
{
public:
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
};

