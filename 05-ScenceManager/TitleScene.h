#pragma once
#include "Scence.h"
#include "Textures.h"
#include "Game.h"
#include "TitleBackGround.h"

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

	CMario* mario;
	CMario* lugi;
	CTitleBackGround* backGround;
	vector<LPGAMEOBJECT> fallingObjs;
	vector<LPGAMEOBJECT> listKoopas;

	DWORD lugi_jump_start;

	bool isShowBush = false;
	bool isShowMenu = false;

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

