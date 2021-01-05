#pragma once
#include "GameObject.h"

#define POINT_ANI_SET		40

#define POINT_100			0
#define POINT_200			1
#define POINT_400			2
#define POINT_800			3
#define POINT_1000			4
#define POINT_2000			5
#define POINT_4000			6
#define POINT_8000			7
#define POINT_1_UP			8

#define POINT_ANI_100		0
#define POINT_ANI_200		1
#define POINT_ANI_400		2
#define POINT_ANI_800		3
#define POINT_ANI_1000		4
#define POINT_ANI_2000		5
#define POINT_ANI_4000		6
#define POINT_ANI_8000		7
#define POINT_ANI_1_UP		8

class CPoint : public CGameObject
{
	int type;
	DWORD appear_start;

public:
	CPoint(float x, float y, int type);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
};

