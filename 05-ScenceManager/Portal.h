#pragma once
#include "GameObject.h"

#define PORTAL_STATE_NORMAL		0
#define PORTAL_STATE_PICK_CARD	1
#define PORTAL_STATE_DONE		2

#define PORTAL_ANI_CARDS			0
#define PORTAL_ANI_PICK_MUSHROOM	1
#define PORTAL_ANI_PICK_STAR		2
#define PORTAL_ANI_PICK_FLOWER		3

#define PORTAL_BBOX_SIZE		26

#define SPRITE_CONGRAT_ID		56010

/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	int scene_id;	// target scene to switch to 
	int idCard;

	int x_start, y_start;

	LPSPRITE congratSpr;
	LPSPRITE cardSpr = NULL;

	DWORD change_start;
	DWORD end_start;

	void SetBoundingBox();
public:
	CPortal(float l, float t, int scene_id);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);
	void Render();
	void SetState(int state);
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	int GetSceneId() { return scene_id; }
};