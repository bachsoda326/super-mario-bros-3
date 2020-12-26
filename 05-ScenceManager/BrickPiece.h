#pragma once
#include "GameObject.h"

#define BRICK_PIECE_ANI_SET	35

#define BRICK_PIECE_SIZE	10


#define BRICK_PIECE_HIGH_SPEED_X	0.1f
#define BRICK_PIECE_LOW_SPEED_X		0.08f
#define BRICK_PIECE_HIGH_SPEED_Y	0.3f
#define BRICK_PIECE_LOW_SPEED_Y		0.15f
#define BRICK_PIECE_GRAVITY			0.001f

class CBrickPiece : public CGameObject
{
	void SetBoundingBox() {};
public:
	CBrickPiece();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
};

