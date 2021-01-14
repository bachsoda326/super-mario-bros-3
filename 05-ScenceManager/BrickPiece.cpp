#include "BrickPiece.h"

CBrickPiece::CBrickPiece()
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(BRICK_PIECE_ANI_SET);
	SetAnimationSet(ani_set);

	isDie = true;
}

void CBrickPiece::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	vy += BRICK_PIECE_GRAVITY * dt;

	x += dx;
	y += dy;

	if (y + BRICK_PIECE_SIZE > 432)
	{
		isDead = true;
		DeleteFrontObjs(coObjects);
	}
}

void CBrickPiece::Render()
{
	animation_set->at(0)->Render(x, y);
}

