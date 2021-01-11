#include "Enemy.h"

void CEnemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;

	if (y > HEIGHT_MAP_1_1) Dead();
}
