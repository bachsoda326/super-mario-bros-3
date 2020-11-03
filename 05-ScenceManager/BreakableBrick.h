#pragma once
#include "Brick.h"

class CBreakableBrick : public CBrick
{
public:
	bool isColision = false;
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
};

