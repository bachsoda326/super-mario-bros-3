#pragma once
#include "Brick.h"

class CBreakableBrick : public CBrick
{
public:
	int type;

	bool isColision = false;

	CBreakableBrick(int type);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
};

