#include "BreakableBrick.h"
#include "PlayScence.h"
#include "PSwitch.h"
#include "BrickPiece.h"
#include "PlayerInfo.h"

CBreakableBrick::CBreakableBrick(int type)
{
	this->type = type;
	SetState(BREAKABLE_BRICK_STATE_NORMAL);

	SetBoundingBox();
}

void CBreakableBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (coin_start == 0 && type == BREAKABLE_BRICK_TYPE_COIN && ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->isBrickToCoin)
	{
		SetState(BREAKABLE_BRICK_STATE_COIN);
		coin_start = GetTickCount();
	}

	if (coin_start != 0 && (GetTickCount() - coin_start) > 5000)
	{
		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->isBrickToCoin = false;
		SetState(BREAKABLE_BRICK_STATE_NORMAL);
		coin_start = 0;
	}

	if (break_start != 0 && (GetTickCount() - break_start) > 5000)
	{
		DeleteFrontObjs(coObjects);
	}
}

void CBreakableBrick::Render()
{	
	int ani = -1;
	switch (state)
	{
	case BREAKABLE_BRICK_STATE_NORMAL: case BREAKABLE_BRICK_STATE_BREAK:
		ani = BREAKABLE_BRICK_ANI_NORMAL;
		break;
	case BREAKABLE_BRICK_STATE_COIN:
		ani = BREAKABLE_BRICK_ANI_COIN;
		break;
	case BREAKABLE_BRICK_STATE_1UP_MUSHROOM_LEFT: case BREAKABLE_BRICK_STATE_1UP_MUSHROOM_RIGHT: case BREAKABLE_BRICK_STATE_P_SWITCH:
		ani = BREAKABLE_BRICK_ANI_HIT;
		break;	
	default:
		break;
	}
	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CBreakableBrick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{		
	case BREAKABLE_BRICK_STATE_BREAK:
		isDie = true;
		isDead = true;
		break_start = GetTickCount();

		for (int i = 0; i < 4; i++)
		{
			CBrickPiece* piece = new CBrickPiece();
			piece->SetPosition(x, y);			
			switch (i)
			{
			case 0:
				piece->vx = -BRICK_PIECE_HIGH_SPEED_X;				
				piece->vy = -BRICK_PIECE_HIGH_SPEED_Y;
				break;
			case 1:
				piece->vx = BRICK_PIECE_HIGH_SPEED_X;
				piece->vy = -BRICK_PIECE_HIGH_SPEED_Y;
				break;
			case 2:
				piece->vx = -BRICK_PIECE_LOW_SPEED_X;
				piece->vy = -BRICK_PIECE_LOW_SPEED_Y;
				break;
			case 3:
				piece->vx = BRICK_PIECE_LOW_SPEED_X;
				piece->vy = -BRICK_PIECE_LOW_SPEED_Y;
				break;
			default:
				break;
			}
			CGame::GetInstance()->GetCurrentScene()->GetFrontObjs()->push_back(piece);
		}
		CPlayerInfo::GetInstance()->AdjustScore(10);

		break;
	case BREAKABLE_BRICK_STATE_1UP_MUSHROOM_LEFT:
		obj = new CMushRoom(x, y, -1, MUSHROOM_TYPE_1_UP);
		CGame::GetInstance()->GetCurrentScene()->GetBehindObjs()->push_back(obj);
		break;
	case BREAKABLE_BRICK_STATE_1UP_MUSHROOM_RIGHT:
		obj = new CMushRoom(x, y, 1, MUSHROOM_TYPE_1_UP);
		CGame::GetInstance()->GetCurrentScene()->GetBehindObjs()->push_back(obj);
		break;
	case BREAKABLE_BRICK_STATE_P_SWITCH:
		obj = new CPSwitch(x, y);
		CGame::GetInstance()->GetCurrentScene()->GetBehindObjs()->push_back(obj);
		break;
	default:
		break;
	}
}

void CBreakableBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	if (state == BREAKABLE_BRICK_STATE_COIN)
	{
		r = x + COIN_BBOX_WIDTH;
		b = y + COIN_BBOX_HEIGHT;
	}
	else
	{		
		r = x + BRICK_BBOX_WIDTH;
		b = y + BRICK_BBOX_HEIGHT;
	}

	CGameObject::GetBoundingBox(l, t, r, b);
}

void CBreakableBrick::SetBoundingBox()
{
	left = x;
	top = y;
	if (state == BREAKABLE_BRICK_STATE_COIN)
	{
		right = x + COIN_BBOX_WIDTH;
		bottom = y + COIN_BBOX_HEIGHT;
	}
	else
	{
		right = x + BRICK_BBOX_WIDTH;
		bottom = y + BRICK_BBOX_HEIGHT;
	}
}