#pragma once

#include <d3dx9.h>
#include "KeyEventHandler.h"
#include "GameObject.h"

class CScene
{
protected:
	// All objs for later grid
	vector<LPGAMEOBJECT> objects;
	// Behind and Front objs for later init (not in grid)
	vector<LPGAMEOBJECT> behindObjs;
	vector<LPGAMEOBJECT> frontObjs;

	CKeyEventHandler * key_handler;
	int id;
	LPCWSTR sceneFilePath;

public:
	CScene(int id, LPCWSTR filePath);

	vector<LPGAMEOBJECT>* GetObjs() { return &objects; }
	vector<LPGAMEOBJECT>* GetFrontObjs() { return &frontObjs; }
	vector<LPGAMEOBJECT>* GetBehindObjs() { return &behindObjs; }
	CKeyEventHandler * GetKeyEventHandler() { return key_handler; }
	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0; 

	int GetSceneId() { return id; }
};
typedef CScene * LPSCENE;


class CScenceKeyHandler : public CKeyEventHandler
{
protected: 
	CScene * scence; 

public:
	virtual void KeyState(BYTE *states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
	CScenceKeyHandler(CScene *s) :CKeyEventHandler() { scence = s; }
};