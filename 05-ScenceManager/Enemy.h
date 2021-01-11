#pragma once
#include "GameObject.h"
// Speed
#define ENEMY_GRAVITY				0.0007f
#define ENEMY_DIE_X_SPEED			0.03f
#define ENEMY_DIE_TAIL_Y_SPEED		0.25f
#define ENEMY_HIDE_TAIL_Y_SPEED		0.28f
#define ENEMY_DIE_Y_SPEED			0.2f

class CEnemy : public CGameObject
{
public:
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render() {};
	void SetState(int state) {};
};

