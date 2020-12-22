#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Box.h"
#include "Ground.h"
#include "QuestionBrick.h"
#include "BreakableBrick.h"
#include "WarpPipe.h"
#include "MushRoom.h"
#include "Leaf.h"
#include "TileMap.h"
#include "Coin.h"
#include "CloudTooth.h"
#include "ParaGoomba.h"


class CPlayScene: public CScene
{
protected: 
	CMario *player;					// A play scene has to have player, right? 

	//vector<LPGAMEOBJECT> objects;

	CTileMap* map;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);

	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	// Cập nhật vị trí camera khi đụng biên map và khi Mario di chuyển // Chưa dùng đến
	void UpdateCamera(int mapWidth, int mapHeight);

	CMario * GetPlayer() { return player; } 

	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

