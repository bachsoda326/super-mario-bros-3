#pragma once
#include "GameObject.h"
#include "WorldStation.h"

#define WORLD_MARIO_BBOX_WIDTH	14
#define WORLD_MARIO_BBOX_HEIGHT	16

class CWorldMario : public CGameObject
{
	CWorldStation* curStation = NULL;
		
	void SetBoundingBox();
public:
	CWorldStation* GetCurrentStation() { return curStation; }
	void SetCurrentStation(CWorldStation* station) { curStation = station; }

	CWorldMario();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	// Giao nhau vs các obj
	void OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs);

	void GoLeft();
	void GoTop();
	void GoRight();
	void GoBottom();
};

