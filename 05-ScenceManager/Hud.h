#pragma once
#include "GameObject.h"
#include "Font.h"
#include"Mario.h"
// BBox
#define FONT_BBOX_WIDTH						8
#define HUD_BBOX_HEIGHT						58
// Sprite
#define SPRITE_HUD_ID						55001
#define SPRITE_CARD_HUD_ID					55002
#define SPRITE_MARIO_PLAYER_ID				55003
#define SPRITE_LUGI_PLAYER_ID				55004
#define SPRITE_FILLED_POWER_ID				55005
#define SPRITE_FILLED_POWER_ARROW_ID		55006
#define SPRITE_POWER_ID						55007
#define SPRITE_POWER_ARROW_ID				55008
#define SPRITE_CARD_EMPTY_ID				55009
#define SPRITE_CARD_MUSHROOM_ID				55010
#define SPRITE_CARD_FLOWER_ID				55011
#define SPRITE_CARD_STAR_ID					55012
#define SPRITE_BBOX_BLACK_ID				90001	
// Animation
#define ANIMATION_FILLED_POWER_ARROW_ID		55001
#define ANIMATION_MUSHROOM_CARD_ID			55002
#define ANIMATION_STAR_CARD_ID				55003
#define ANIMATION_FLOWER_CARD_ID			55004
// Time
#define START_TIME							300
// Others
#define LIFE_NUM_COUNT						2
#define SCORES_NUM_COUNT					7
#define TIME_NUM_COUNT						3
#define WORLD_1								1
#define MAX_POWER_STACK						7
#define MAX_CARDS							3
#define HUD_X								35
#define HUD_Y								200
#define CARDHUD_DISTANCE_X					160
#define CARD_DISTANCE_X						161
#define HUD_DISTANCE_X						52
#define HUD_DISTANCE_X_1					53
#define HUD_DISTANCE_Y						7
#define HUD_DISTANCE_Y_1					15
#define HUD_DISTANCE_Y_2					2
#define ITEM_DISTANCE						8
#define CARD_DISTANCE						24
#define WORLD_DISTANCE_X					36
#define ICON_DISTANCE_X						4
#define LIFE_DISTANCE_X						44
#define TIME_DISTANCE_X						124
#define MONEY_DISTANCE_X					148

//#define HUD_ANI_SET		39

class CHud : public CGameObject
{
	CFont* font;

	int world;
	int time = 0;
	int timeRemain = 0;
	int power = 0;
	CMario* mario = NULL;

	LPSPRITE hud;
	LPSPRITE cardsHud;
	LPSPRITE background;

	LPSPRITE worldSpr;
	LPSPRITE iconSpr;
	vector<LPSPRITE> lifeSpr;
	vector<LPSPRITE> moneySpr;
	vector<LPSPRITE> scoresSpr;
	vector<LPSPRITE> powerSpr;
	vector<LPSPRITE> filledPowerSpr;
	LPANIMATION filledPowerArrowAni;
	vector<LPSPRITE> cardsSpr;
	LPANIMATION cardsAni = NULL;
	vector<LPSPRITE> timeSpr;

public:
	CHud();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();

	void EndScene();
};

