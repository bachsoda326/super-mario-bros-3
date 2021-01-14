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
	float topCamera = CCamera::GetInstance()->GetBound().top;
	float rightCamera = CCamera::GetInstance()->GetBound().right;
	float bottomCamera = CCamera::GetInstance()->GetBound().bottom;

	if (!isDie)
	{
		if (!isActive)
		{
			if (xSpawn + right - left >= leftCamera && xSpawn <= rightCamera && ySpawn + top - bottom >= topCamera && ySpawn <= bottomCamera - 24)
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
			float height = CGame::GetInstance()->GetScreenHeight();
			if (x + right - left + width / 4 <= leftCamera || x - width / 4 >= rightCamera || y + top - bottom + height / 8 <= topCamera || y - height / 8 >= bottomCamera - 24)
			{
				isActive = false;
				canActive = false;
				SetPosition(xSpawn, ySpawn);
			}
		}
	}

	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;

	switch (((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetMap()->GetId())
	{
	case MAP_1_1:
		if (y > HEIGHT_MAP_1_1) 
		{
			Dead();
			DeleteObjs(coObjects);
		}
		break;
	default:
		break;
	}
	
}
