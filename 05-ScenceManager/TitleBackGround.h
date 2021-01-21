#pragma once
#include "GameObject.h"

class CTitleBackGround : public CGameObject
{
	LPSPRITE scrollCurtain;
	LPSPRITE ground;
	vector<LPSPRITE> backGrounds;
	int curBackGround;

public:
	CTitleBackGround();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
};

