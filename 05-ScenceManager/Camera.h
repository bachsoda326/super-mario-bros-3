#pragma once
#include <d3dx9.h>
#include "Game.h"
#include "Mario.h"
#include "Constants.h"

class CCamera
{
	static CCamera* __instance;

	int width, height;
	int widthMap, heightMap;
	int leftMap, topMap, rightMap, bottomMap;
	bool isStatic = true;

	D3DXVECTOR3 position;
public:
	CCamera();
	static CCamera* GetInstance();

	void SetPosition(D3DXVECTOR3 pos);
	void SetPosition(float x, float y);
	void SetMapSize(int left, int top, int right, int bottom, int width, int height);

	void Update(CMario *player);

	D3DXVECTOR3 GetPosition();
	int GetWidth();
	int GetHeight();
	// Trả về biên của camera thông qua RECT
	RECT GetBound();
	void SetIsStatic(bool isStatic) { this->isStatic = isStatic; };

	~CCamera();
};

