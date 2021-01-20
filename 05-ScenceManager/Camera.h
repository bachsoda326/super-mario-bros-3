#pragma once
#include <d3dx9.h>
#include "Game.h"
#include "Mario.h"
#include "Constants.h"

class CCamera
{
	static CCamera* __instance;

	int width, height;
	int widthMap = -1, heightMap = -1;
	int leftMap = -1, topMap = -1, rightMap = -1, bottomMap = -1;
	bool isStatic = true;
	bool isMoving = true;

	D3DXVECTOR3 position;
public:
	CCamera();
	static CCamera* GetInstance();

	void SetPosition(D3DXVECTOR3 pos);
	void SetPosition(float x, float y);
	void SetMapSize(int left, int top, int right, int bottom, int width, int height);

	void Update(DWORD dt);

	D3DXVECTOR3 GetPosition();
	int GetWidth();
	int GetHeight();
	int GetLeftMap() { return leftMap; }
	int GetTopMap() { return topMap; }
	int GetRightMap() { return rightMap; }
	int GetBottomMap() { return bottomMap; }
	int GetHeightMap() { return heightMap; }
	// Trả về biên của camera thông qua RECT
	RECT GetBound();
	void SetIsStatic(bool isStatic) { this->isStatic = isStatic; };
	void SetIsMoving(bool isMoving);

	~CCamera();
};

