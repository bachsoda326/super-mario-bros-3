#include "Camera.h"

CCamera* CCamera::__instance = NULL;

CCamera::CCamera()
{
	position = D3DXVECTOR3(0, 0, 0);

	width = CGame::GetInstance()->GetScreenWidth();
	height = CGame::GetInstance()->GetScreenHeight();
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

void CCamera::SetMapSize(int width, int height)
{
	mapWidth = width;
	mapHeight = height;
}

void CCamera::Update(CMario* player)
{
	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);
	SetPosition(cx, cy);

	if (GetBound().left < 0)
	{
		//vi position cua Camera::GetInstance() ma chinh giua Camera::GetInstance()
		//luc nay o vi tri goc ben trai cua the gioi thuc
		SetPosition(GetWidth() / 2, GetPosition().y);
	}

	if (GetBound().right > mapWidth)
	{
		//luc nay cham goc ben phai cua the gioi thuc
		SetPosition(mapWidth - GetWidth() / 2, GetPosition().y);
	}

	if (GetBound().top < 0)
	{
		//luc nay cham goc tren the gioi thuc
		SetPosition(GetPosition().x, GetHeight() / 2);
	}

	if (GetBound().bottom > mapHeight / 2)
	{
		//luc nay cham day cua the gioi thuc
		SetPosition(GetPosition().x, mapHeight - GetHeight() / 2);
	}
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