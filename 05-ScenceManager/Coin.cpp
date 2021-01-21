#include "Coin.h"
#include "Game.h"

CCoin::CCoin()
{
	SetBoundingBox();
}

CCoin::CCoin(float x, float y, bool isInBrick)
{
	start_x = x;
	start_y = y;
	SetPosition(x, y);
	this->isInBrick = isInBrick;
	if (this->isInBrick)
	{
		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(COIN_ANI_SET);
		SetAnimationSet(ani_set);
		isDie = true;
	}
	vy = -COIN_Y_SPEED;

	SetBoundingBox();
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isInBrick)
	{
		CGameObject::Update(dt, coObjects);

		x += dx;
		y += dy;

		vy += COIN_GRAVITY * dt;

		// Delete coin
		if (y >= start_y)
		{
			AddPoint();
			DeleteBehindObjs(coObjects);
		}
	}
}

void CCoin::Render()
{
	int ani = 0;
	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + COIN_BBOX_WIDTH;
	b = y + COIN_BBOX_HEIGHT;

	CGameObject::GetBoundingBox(l, t, r, b);
}

void CCoin::SetBoundingBox()
{
	left = x;
	top = y;
	right = x + COIN_BBOX_WIDTH;
	bottom = y + COIN_BBOX_HEIGHT;
}
