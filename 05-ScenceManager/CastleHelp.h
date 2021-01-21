#pragma once
#include "GameObject.h"

#define CASTLE_DISAPPEAR_TIME		300
#define CASTLE_HELP_TIME			1000

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

