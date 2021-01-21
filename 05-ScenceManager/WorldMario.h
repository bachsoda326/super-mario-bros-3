#pragma once
#include "GameObject.h"
#include "WorldStation.h"
// BBox
#define WORLD_MARIO_BBOX_WIDTH	14
#define WORLD_MARIO_BBOX_HEIGHT	16
// Speed
#define WORLD_MARIO_X_SPEED		0.15f

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
	// Intersect with objs
	void OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs);

	void GoLeft();
	void GoTop();
	void GoRight();
	void GoBottom();
};

