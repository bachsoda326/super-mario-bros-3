#include "Portal.h"
#include "PlayScence.h"
#include "PlayerInfo.h"

CPortal::CPortal(float l, float t, int scene_id )
{
	this->scene_id = scene_id;
	x_start = l; 
	y_start = t;
	
	idCard = SPRITE_CARD_MUSHROOM_ID;
	SetState(PORTAL_STATE_NORMAL);

	congratSpr = CSprites::GetInstance()->Get(SPRITE_CONGRAT_ID);
	change_start = GetTickCount();

	SetBoundingBox();
}

void CPortal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	CGameObject::Update(dt);

	y += dy;

	// Change cards
	if (state == PORTAL_STATE_NORMAL && GetTickCount() - change_start > 100)
	{
		if (idCard == SPRITE_CARD_MUSHROOM_ID)
			idCard = SPRITE_CARD_STAR_ID;
		else if (idCard == SPRITE_CARD_STAR_ID)
			idCard = SPRITE_CARD_FLOWER_ID;
		else if (idCard == SPRITE_CARD_FLOWER_ID)
			idCard = SPRITE_CARD_MUSHROOM_ID;
		change_start = GetTickCount();
	}

	if (state == PORTAL_STATE_PICK_CARD)
	{
		if (((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer()->IsEndScene())
		{
			SetState(PORTAL_STATE_DONE);
		}
	}

	if (end_start != 0 && GetTickCount() - end_start > 3000)
	{
		CGame::GetInstance()->SwitchScene(scene_id);
	}
}

void CPortal::Render()
{
	if (state != PORTAL_STATE_DONE)
	{
		int ani = -1;

		switch (state)
		{
		case PORTAL_STATE_NORMAL:
			ani = PORTAL_ANI_CARDS;
			animation_set->at(ani)->Render(x, y);
			break;
		case PORTAL_STATE_PICK_CARD:
			switch (idCard)
			{
			case SPRITE_CARD_MUSHROOM_ID:
				ani = PORTAL_ANI_PICK_MUSHROOM;
				break;
			case SPRITE_CARD_STAR_ID:
				ani = PORTAL_ANI_PICK_STAR;
				break;
			case SPRITE_CARD_FLOWER_ID:
				ani = PORTAL_ANI_PICK_FLOWER;
				break;
			default:
				break;
			}
			animation_set->at(ani)->Render(x + 5, y);
			break;		
		default:
			break;
		}
	}
	else
	{		
		congratSpr->DrawSprite(35, 10);
		cardSpr->DrawSprite(207, 54);
	}

	//RenderBoundingBox();
}

void CPortal::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case PORTAL_STATE_PICK_CARD:
		vy = -0.08f;
		break;
	case PORTAL_STATE_DONE:		
		cardSpr = CSprites::GetInstance()->Get(idCard);
		CPlayerInfo::GetInstance()->AddCard(idCard);
		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->EndScene();
		end_start = GetTickCount();
		vy = 0;
		break;
	default:
		break;
	}

}

void CPortal::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + PORTAL_BBOX_SIZE;
	b = y + PORTAL_BBOX_SIZE;

	CGameObject::GetBoundingBox(left, top, right, bottom);
}


void CPortal::SetBoundingBox()
{
	left = x;
	top = y;
	right = x + PORTAL_BBOX_SIZE;
	bottom = y + PORTAL_BBOX_SIZE;
}