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
			// Respawn in region
			if (xSpawn + right - left >= leftCamera && xSpawn <= rightCamera && ySpawn + top - bottom >= topCamera && ySpawn <= bottomCamera - CAMERA_DISTANCE_Y)
			{
				if (canActive)
				{
					isActive = true;
					SetState(ENEMY_STATE_RESPAWN);
				}
			}
			// CanActive when move out of region
			else
				canActive = true;
			return;
		}
		else
		{
			float width = CGame::GetInstance()->GetScreenWidth();
			float height = CGame::GetInstance()->GetScreenHeight();
			if (x + right - left + width / 4 <= leftCamera || x - width / 4 >= rightCamera || y + top - bottom + height / 4 <= topCamera || y - height / 8 >= bottomCamera - CAMERA_DISTANCE_Y)
			{
				isActive = false;
				canActive = false;
				SetPosition(xSpawn, ySpawn);
				// Back to grid if Koopas is holded and release far
				if (!isInGrid)
				{
					DeleteBehindObjs(coObjects, false);
					isInGrid = true;
				}
			}
		}
	}

	CGameObject::Update(dt, coObjects);

	MoveThrough(OBJ_MOVE_XY);

	// Fall
	if (y > CCamera::GetInstance()->GetTopMap() + CCamera::GetInstance()->GetHeightMap())
	{
		Dead();
		DeleteObjs(coObjects);
	}	
}
