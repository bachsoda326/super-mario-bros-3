#pragma once
#include <d3dx9.h>

class CCamera
{
public:
	CCamera();
	static CCamera* GetInstance();

	void SetPosition(D3DXVECTOR3 pos);
	void SetPosition(float x, float y);

	D3DXVECTOR3 GetPosition();
	int GetWidth();
	int GetHeight();
	// Trả về biên của camera thông qua RECT
	RECT GetBound();

	~CCamera();

private:
	static CCamera* __instance;

	int width, height;

	D3DXVECTOR3 position;
};

