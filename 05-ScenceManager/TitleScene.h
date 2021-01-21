#pragma once
#include "Scence.h"
#include "Textures.h"
#include "Game.h"
#include "TitleBackGround.h"

#define SPRITE_TITLE_BACKGROUND_ID		90001
#define SPRITE_TITLE_ARROW_ID			90101
#define ANIMATION_NUM_3_ID				90201

#define ONE_PLAYER_TYPE		1
#define TWO_PLAYER_TYPE		2

class CTitleScene : public CScene
{
protected:
	/*CMario* mario;
	CMario lugi;
	CGoomba* goomba;
	CKoopas* koopas;
	vector<CKoopas*> listKoopas;
	vector<LPGAMEOBJECT> fallingObjs;
	vector <LPGAMEOBJECT> objs;

	CTitleBackGround* backGround;
	vector<LPSPRITE> bushSprs;*/

	LPSPRITE backgroundSpr = NULL;
	LPSPRITE onePlayerArrowSpr = NULL;
	LPSPRITE twoPlayerArrowSpr = NULL;
	LPANIMATION num3Ani = NULL;
		
	int playType = ONE_PLAYER_TYPE;
	bool isShowBush = false;
	bool isShowMenu = false;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_OBJECTS(string line);

public:
	CTitleScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();	

	void SetPlayType(int num) { playType = num; }
	int GetPlayType() { return playType; }
};

class CTitleSceneKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CTitleSceneKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

