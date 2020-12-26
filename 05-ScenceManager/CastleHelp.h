#pragma once
#include "GameObject.h"

class CCastleHelp : public CGameObject
{
	DWORD help_start;
	bool isHelp = false;

	void SetBoundingBox() {};
public:
	CCastleHelp();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
};

