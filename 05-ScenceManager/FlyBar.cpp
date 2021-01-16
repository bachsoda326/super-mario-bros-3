#include "FlyBar.h"
#include "Camera.h"
#include "PlayScence.h"

CFlyBar::CFlyBar()
{
	isActive = false;

	SetBoundingBox();
}

void CFlyBar::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{	
	float rightCamera = CCamera::GetInstance()->GetBound().right;
	if (!isActive && x <= rightCamera)
	{
		isActive = true;
		SetState(FLY_BAR_STATE_SPAWN);
	}

	if (!isActive || isDead) return;

	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;

	// Simple fall down
	if (state == FLY_BAR_STATE_FALL)
	{
		vy += FLY_BAR_GRAVITY * dt;
		/*CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		mario->vy = vy;*/
	}		

	if (y > HEIGHT_MAP_1_4)
	{
		Dead();
		DeleteObjs(coObjects);
		return;
	}
}

void CFlyBar::Render()
{
	animation_set->at(0)->Render(x, y);

	RenderBoundingBox();
}

void CFlyBar::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case FLY_BAR_STATE_SPAWN:
		vx = -FLY_BAR_X_SPEED;
		break;
	case FLY_BAR_STATE_FALL:
		vx = 0;
		break;
	default:
		break;
	}
}

void CFlyBar::SetBoundingBox()
{
	left = x;
	top = y;
	right = x + FLY_BAR_BBOX_WIDTH;
	bottom = y + FLY_BAR_BBOX_HEIGHT;
}

void CFlyBar::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + FLY_BAR_BBOX_WIDTH;
	b = y + FLY_BAR_BBOX_HEIGHT;

	CGameObject::GetBoundingBox(l, t, r, b);
}
