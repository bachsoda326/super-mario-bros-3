#pragma once
#include "GameObject.h"
// BBox
#define BRICK_BBOX_WIDTH	16
#define BRICK_BBOX_HEIGHT	16
// Type
#define BRICK_NORMAL		0
#define BRICK_ITEM			1

class CBrick : public CGameObject
{
	bool isColLeft = false;
	bool isColRight = false;

	void SetBoundingBox();
public:
	float start_x = -1;			// Init position of brick
	float start_y = -1;
	bool isUp = false;			// Use for col with Koopas when Mario jump to hit this

	CBrick();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	bool GetIsColLeft() { return isColLeft; }
	bool GetIsColRight() { return isColRight; }

	// Intersect with objs
	void OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs);
};