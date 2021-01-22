#pragma once
#include "GameObject.h"
// Others
#define SPRITE_TITLE_BACKGROUND_ID		90001
#define SPRITE_TITLE_ARROW_ID			90101
#define ANIMATION_NUM_3_ID				90201
#define SPRITE_SCROLL_CURTAIN_ID	90003
#define SPRITE_GROUND_ID			90004
#define SPRITE_BACKGROUND_ID		90002
// Speed
#define CURTAIN_SCROLL_SPEED		0.05f
// Type
#define ONE_PLAYER_TYPE		1
#define TWO_PLAYER_TYPE		2

class CTitleBackGround : public CGameObject
{
	LPSPRITE scrollCurtain;
	LPSPRITE ground;
	vector<LPSPRITE> backGrounds;
	LPSPRITE onePlayerArrowSpr = NULL;
	LPSPRITE twoPlayerArrowSpr = NULL;
	LPANIMATION num3Ani = NULL;
	int curBackGround = 0;

public:
	bool isScene1Begin = false;
	bool isScene1End = false;
	bool isScene2Begin = false;
	bool isScene2End = false;
	bool isScene3Begin = false;
	bool isScene3End = false;
	DWORD scene3_start;

	int playType = ONE_PLAYER_TYPE;

	CTitleBackGround();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
};

