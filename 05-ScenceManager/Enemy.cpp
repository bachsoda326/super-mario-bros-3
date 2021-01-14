#include "Enemy.h"
#include "Camera.h"
#include "PlayScence.h"

CEnemy::CEnemy()
{
	isActive = false;
}

void CEnemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float leftCamera = CCamera::GetInstance()->GetBound().left;
	float rightCamera = CCamera::GetInstance()->GetBound().right;

	if (!isActive)
	{
		if (xSpawn + right - left >= leftCamera && xSpawn <= rightCamera)
		{
			if (canActive) 
			{ 
				isActive = true; 
				SetState(ENEMY_STATE_RESPAWN);
			}
		}
		else
			canActive = true;
		return;
	}
	else
	{
		float width = CGame::GetInstance()->GetScreenWidth();
		if (x + right - left + width / 4 <= leftCamera || x - width / 4 >= rightCamera)
		{
			isActive = false;
			canActive = false;
			SetPosition(xSpawn, ySpawn);			
		}
	}

	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;

	switch (((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetMap()->GetId())
	{
	case MAP_1_1:
		if (y > HEIGHT_MAP_1_1) Dead();
		break;
	default:
		break;
	}
	
}
