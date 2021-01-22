#pragma once
#include "Scence.h"
#include "Textures.h"
#include "Game.h"
#include "TitleBackGround.h"

#define SPRITE_BUSH_LEFT_ID			90005
#define SPRITE_BUSH_RIGHT_ID		90006
// Speed
#define KOOPAS_LUGI_KICK_SPEED		0.15f
#define MARIO_SCENE1_X_SPEED		0.01f
#define MARIO_SCENE1_X_SPEED_1		0.04f
#define LUGI_SCENE1_X_SPEED			0.07f
#define LUGI_SCENE1_Y_SPEED			0.22f
#define MARIO_SKID_SPEED			0.002f
#define LUGI_X_SPEED				0.05f
#define MARIO_EAT_LEAF_X_SPEED		0.08f
#define MARIO_EAT_LEAF_Y_SPEED		0.4f
// Time
#define KOOPAS_SPAWN_TIME			1500
#define KOOPAS_SPAWN_TIME_2			2500
#define LOOK_KOOPAS_TIME			1000
#define LOOK_KOOPAS_TIME_2			1500
#define MARIO_LOOKUP_TIME			1000
#define MARIO_DUCK_TIME				300
#define MARIO_BONK_TIME				150
#define MARIO_WAG_TIME				1800
#define MARIO_KICK_WALK_TIME		1000
// Others
#define LUGI_SCENE1_X_JUMP			90
#define BUSH_X						230
#define BUSH_Y						105
#define MARIO_SKID_X				220
#define MARIO_LOOK_KOOPAS_X			180
#define GROUND_Y					220
#define MARIO_RUN_KOOPAS_X			100
#define MARIO_JUMP_KOOPAS_X			80
#define LUGI_KICK_KOOPAS_X			270
#define LUGI_RUN_KOOPAS_X			80

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

	CMario* mario = NULL;
	CMario* lugi = NULL;
	CTitleBackGround* backGround = NULL;
	CKoopas* koopas = NULL;
	vector<LPGAMEOBJECT> fallingObjs;
	vector<LPGAMEOBJECT> listKoopas;
	vector<LPSPRITE> bushSprs;

	DWORD lugi_jump_start = 0;
	DWORD kick_walk_start = 0;
	DWORD look_koopas_start = 0;
	DWORD look_koopas_2_start = 0;
	DWORD spawn_koopas_start = 0;

	bool isShowBush = false;
	bool isShowKoopas = false;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);

public:
	CTitleScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	
	CTitleBackGround* GetBackGround() { return backGround; }
	CMario* GetMario() { return mario; }
	CMario* GetLugi() { return lugi; }
	void HandleMario();
	void HandleLugi();
};

class CTitleSceneKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CTitleSceneKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

