#pragma once
#include "GameObject.h"
// BBox
#define BOOMERANG_BBOX_SIZE			16
// Time
#define BOOMERANG_CAN_CATCH_TIME	500
// Others
#define PI							3.14f
#define BOOMERANG_ANI_SET			44
#define BOOMERANG_X_DISTANCE		70
#define BOOMERANG_X_CENTER			80
#define BOOMERANG_Y_CENTER			30
#define BOOMERANG_DIVISION			2000

class CBoomerang : public CGameObject
{
	float start_x = -1;
	float start_y = -1;
	bool canCatch = false;

	DWORD start_time = 0;

	void SetBoundingBox();

public:
	CBoomerang(float x, float y, int direction);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	// Intersect with objs
	void OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs);
};

