#include "Piranha.h"
#include "PlayScence.h"

CPiranha::CPiranha(int type, float x, float y)
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(PIRANHA_ANI_SET);
	SetAnimationSet(ani_set);

	this->type = type;
	start_bottom = y;
	switch (type)
	{
	case PIRANHA_NOR:
		height = PIRANHA_NOR_BBOX_HEIGHT;
		break;
	case PIRANHA_FIRE:
		height = PIRANHA_FIRE_BBOX_HEIGHT;
		break;
	case PIRANHA_FIRE_RED:
		height = PIRANHA_FIRE_RED_BBOX_HEIGHT;		
		break;
	default:
		break;
	}
	start_top = start_bottom - height;
	SetPosition(x, y);

	if (this->type != PIRANHA_NOR)
	{
		/*bullet = new CBullet();
		bullet->isEnemy = true;
		CGame::GetInstance()->GetCurrentScene()->GetObjs()->push_back(bullet);*/
	}

	SetBoundingBox();
}

void CPiranha::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	CGameObject::Update(dt, coObjs);
		
	MoveThrough(OBJ_MOVE_Y);

	if (state == PIRANHA_STATE_DIE)
	{
		if (GetTickCount() - explode_start > PIRANHA_EXPLODE_TIME)
			isDead = true;
		return;
	}

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float distanceX = (x + PIRANHA_BBOX_WIDTH / 2) - (mario->x + MARIO_BIG_BBOX_WIDTH / 2);
	float distanceY = (y - (mario->y + MARIO_BIG_BBOX_HEIGHT / 2));
	// Piranha head follow Mario	
	if (distanceX > 0)
	{
		xReverse = false;
		nx = -1;
	}
	else
	{
		xReverse = true;
		nx = 1;
	}
	if (distanceY < 0)
		isUp = false;
	else
		isUp = true;
	
	// Wait top
	if (y < start_top)
	{
		y = start_top;
		wait_up_start = GetTickCount();
		if (type != PIRANHA_NOR)
			wait_fire_start = GetTickCount();
		vy = 0;
	}
	// Wait bottom
	if (y > start_bottom + PIRANHA_BOTTOM_DISTANCE_1)
	{
		y = start_bottom + PIRANHA_BOTTOM_DISTANCE_1;
		wait_down_start = GetTickCount();
		SetState(PIRANHA_STATE_HIDE);
	}
	// Go down
	if (wait_up_start != 0 && (type != PIRANHA_NOR && GetTickCount() - wait_up_start > PIRANHA_FIRE_UP_TIME || type == PIRANHA_NOR && GetTickCount() - wait_up_start > PIRANHA_NOR_UP_TIME))
	{
		wait_up_start = 0;
		vy = PIRANHA_Y_SPEED;
	}
	// In region
	if (abs(distanceX) >= PIRANHA_MIN_DISTANCE_X && abs(distanceX) <= PIRANHA_MAX_DISTANCE_X)
	{
		// First when in region -> go up
		if (wait_up_start == 0 && wait_down_start == 0 && vy == 0)
			SetState(PIRANHA_STATE_NORMAL);
		// Go up after go down done
		if (wait_down_start != 0 && (type != PIRANHA_NOR && GetTickCount() - wait_down_start > PIRANHA_FIRE_DOWN_TIME || type == PIRANHA_NOR && GetTickCount() - wait_down_start > PIRANHA_NOR_DOWN_TIME))
		{
			wait_down_start = 0;
			SetState(PIRANHA_STATE_NORMAL);
		}
		// Fire bullet
		if (type != PIRANHA_NOR && wait_fire_start != 0 && wait_up_start != 0 && GetTickCount() - wait_fire_start > PIRANHA_WAIT_FIRE_TIME)
		{
			CBullet* bullet = new CBullet();
			bullet->isEnemy = true;
			CGame::GetInstance()->GetCurrentScene()->GetFrontObjs()->push_back(bullet);

			wait_fire_start = 0;
			if (nx < 0)
			{
				bullet->SetPosition(x, y + PIRANHA_BBOX_HEAD_HEIGHT);
			}
			else
			{
				bullet->SetPosition(right - BULLET_BBOX_WIDTH / 2, y + PIRANHA_BBOX_HEAD_HEIGHT);
			}
			bullet->vx = nx * BULLET_ENEMY_X_SPEED;
			// Fire bullet near or far
			if (abs(distanceX) <= PIRANHA_FAR_DISTANCE_X)
			{
				if (isUp)
					bullet->vy = -BULLET_ENEMY_NEAR_Y_SPEED;
				else
					bullet->vy = BULLET_ENEMY_NEAR_Y_SPEED;
			}
			else
			{
				if (isUp)
					bullet->vy = -BULLET_ENEMY_FAR_Y_SPEED;
				else
					bullet->vy = BULLET_ENEMY_FAR_Y_SPEED;
			}
			bullet->SetState(BULLET_STATE_FIRE);
		}
	}	
}

void CPiranha::Render()
{
	int ani = PIRANHA_ANI_NOR;
	if (state == PIRANHA_STATE_DIE)
		ani = PIRANHA_ANI_DIE;
	else
	{
		switch (type)
		{
		case PIRANHA_NOR:
			ani = PIRANHA_ANI_NOR;
			break;
		case PIRANHA_FIRE:
			if (isUp)
				ani = PIRANHA_ANI_FIRE_UP;
			else
				ani = PIRANHA_ANI_FIRE_DOWN;
			break;
		case PIRANHA_FIRE_RED:
			if (isUp)
				ani = PIRANHA_ANI_FIRE_RED_UP;
			else
				ani = PIRANHA_ANI_FIRE_RED_DOWN;
			break;
		default:
			break;
		}
	}
	
	animation_set->at(ani)->Render(x, y, xReverse, yReverse);

	RenderBoundingBox();
}

void CPiranha::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case PIRANHA_STATE_NORMAL:
		vy = -PIRANHA_Y_SPEED;
		break;
	case PIRANHA_STATE_HIDE:
		vy = 0;
		break;
	case PIRANHA_STATE_DIE:
		AddPoint();
		vy = 0;
		isDie = true;
		SetPosition(x + PIRANHA_BBOX_WIDTH / 5, y);
		explode_start = GetTickCount();
		break;
	default:
		break;
	}
}

void CPiranha::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - PIRANHA_BBOX_HEAD_HEIGHT;
	t = y;
	r = l + PIRANHA_BBOX_WIDTH + 2 * PIRANHA_BBOX_HEAD_HEIGHT;
	b = start_bottom - PIRANHA_BOTTOM_DISTANCE;

	CGameObject::GetBoundingBox(l, t, r, b);
}

void CPiranha::SetBoundingBox()
{
	left = x - PIRANHA_BBOX_HEAD_HEIGHT;
	top = y;
	right = left + PIRANHA_BBOX_WIDTH + 2 * PIRANHA_BBOX_HEAD_HEIGHT;
	bottom = start_bottom - PIRANHA_BOTTOM_DISTANCE;
}


