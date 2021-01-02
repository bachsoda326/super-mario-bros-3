#pragma once
#include "GameObject.h"
#include "Font.h"

#define LIFE_NUM_COUNT		2
#define SCORES_NUM_COUNT	7
#define TIME_NUM_COUNT		3

#define START_TIME			3000
#define WORLD_1				1
#define MAX_POWER_STACK		7

#define FONT_BBOX_WIDTH					8
#define HUD_BBOX_HEIGHT					58
#define SPRITE_HUD_ID					55001
#define SPRITE_CARD_HUD_ID				55002
#define SPRITE_MARIO_PLAYER_ID			55003
#define SPRITE_LUGI_PLAYER_ID			55004
#define SPRITE_FILLED_POWER_ID			55005
#define SPRITE_FILLED_POWER_ARROW_ID	55006
#define SPRITE_POWER_ID					55007
#define SPRITE_POWER_ARROW_ID			55008
#define SPRITE_BBOX_BLACK_ID			90001	

class CHud : public CGameObject
{
	CFont* font;

	int world;
	int time = 0;
	int timeRemain = 0;

	LPSPRITE hud;
	LPSPRITE cardsHud;
	LPSPRITE background;

	LPSPRITE worldSpr;
	vector<LPSPRITE> lifeSpr;
	vector<LPSPRITE> moneySpr;
	vector<LPSPRITE> scoresSpr;
	vector<LPSPRITE> powerSpr;
	vector<LPSPRITE> filledPowerSpr;
	vector<LPSPRITE> cardsSpr;
	vector<LPSPRITE> timeSpr;

public:
	CHud();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
};

