#pragma once
#include "GameObject.h"
#include "WorldStation.h"

#define WORLD_MARIO_BBOX_WIDTH	16
#define WORLD_MARIO_BBOX_HEIGHT	16

class CWorldMario : public CGameObject
{
	CWorldStation* curStation = NULL;

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();

public:
	CWorldStation* GetCurrentStation() { return curStation; }
	void SetCurrentStation(CWorldStation* station) { curStation = station; }

	void GetBoundingBox(float& l, float& t, float& r, float& b);
	// Giao nhau vs các obj
	void OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs);
};

