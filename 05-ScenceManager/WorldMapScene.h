#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "Brick.h"
#include "TileMap.h"
#include "WorldBush.h"
#include "CastleHelp.h"
#include "WorldHammer.h"
#include "WorldMario.h"

class CWorldMapScene : public CScene
{
protected:
	CWorldMario* player;					// A play scene has to have player, right? 

	//vector<LPGAMEOBJECT> objects;

	CTileMap* map;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);

public:
	bool isBrickToCoin = false;

	CWorldMapScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();	

	CWorldMario* GetPlayer() { return player; }
	CTileMap* GetMap() { return map; }

	//friend class CPlayScenceKeyHandler;
};

class CWorldMapSceneKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CWorldMapSceneKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};