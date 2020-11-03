#include "Camera.h"
#include "Constants.h"

CCamera* CCamera::__instance = NULL;

CCamera::CCamera()
{
	position = D3DXVECTOR3(0, 0, 0);

	width = SCREEN_WIDTH;
	height = SCREEN_HEIGHT;
}

CCamera* CCamera::GetInstance()
{
	if (__instance == NULL) __instance = new CCamera();
	return __instance;
}

void CCamera::SetPosition(D3DXVECTOR3 pos)
{
	position = pos;
}

void CCamera::SetPosition(float x, float y)
{
	SetPosition(D3DXVECTOR3(x, y, 0));
}

D3DXVECTOR3 CCamera::GetPosition()
{
	return position;
}

int CCamera::GetWidth()
{
	return width;
}

int CCamera::GetHeight()
{
	return height;
}

RECT CCamera::GetBound()
{
	RECT bound;

	bound.left = position.x - width / 2;
	bound.right = bound.left + width;
	bound.top = position.y - height / 2;
	bound.bottom = bound.top + height;

	return bound;
}

CCamera::~CCamera()
{

}