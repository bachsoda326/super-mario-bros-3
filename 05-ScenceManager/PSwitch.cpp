#include "PSwitch.h"
#include "PlayScence.h"

CPSwitch::CPSwitch(float x, float y)
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(PSWITCH_ANI_SET);
	SetAnimationSet(ani_set);

	SetState(PSWITCH_STATE_NORMAL);
	SetPosition(x, y - 16);
}

void CPSwitch::Render()
{
	int ani = -1;
	switch (state)
	{
	case PSWITCH_STATE_NORMAL:
		ani = PSWITCH_ANI_NORMAL;
		break;
	case PSWITCH_STATE_HIT:
		ani = PSWITCH_ANI_HIT;
		break;
	default:
		break;
	}
	animation_set->at(ani)->Render(x, y);
}

void CPSwitch::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case PSWITCH_STATE_HIT:
		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->isBrickToCoin = true;
		break;
	default:
		break;
	}
}

void CPSwitch::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + PSWITCH_BBOX_WIDTH;
	b = y + PSWITCH_BBOX_HEIGHT;

	CGameObject::GetBoundingBox(l, t, r, b);
}
