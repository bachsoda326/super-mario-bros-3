#pragma once
#include "GameObject.h"
// State
#define PORTAL_STATE_NORMAL			0
#define PORTAL_STATE_PICK_CARD		1
#define PORTAL_STATE_DONE			2
// Animation
#define PORTAL_ANI_CARDS			0
#define PORTAL_ANI_PICK_MUSHROOM	1
#define PORTAL_ANI_PICK_STAR		2
#define PORTAL_ANI_PICK_FLOWER		3
// BBox
#define PORTAL_CHANGE_CARD_TIME		100
#define PORTAL_END_SCENE_TIME		3000
// Speed
#define PORTAL_Y_SPEED				0.08f
// Time
#define PORTAL_BBOX_SIZE			26
// Others
#define SPRITE_CONGRAT_ID			56010
#define CONGRAT_X					35
#define CONGRAT_Y					10
#define CONGRAT_CARD_X				207
#define CONGRAT_CARD_Y				54
#define PORTAL_DISTANCE_X			5

class CPortal : public CGameObject
{
	int scene_id;	// Target scene to switch to 
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