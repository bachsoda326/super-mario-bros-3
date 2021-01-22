#include "TitleBackGround.h"
#include "Game.h"

CTitleBackGround::CTitleBackGround()
{
	CSprites* sprites = CSprites::GetInstance();
	scrollCurtain = sprites->Get(SPRITE_SCROLL_CURTAIN_ID);
	ground = sprites->Get(SPRITE_GROUND_ID);
	onePlayerArrowSpr = CSprites::GetInstance()->Get(SPRITE_TITLE_ARROW_ID);
	twoPlayerArrowSpr = CSprites::GetInstance()->Get(SPRITE_TITLE_ARROW_ID);
	num3Ani = CAnimations::GetInstance()->Get(ANIMATION_NUM_3_ID);
	for (int i = 0; i < 2; i++)
	{
		backGrounds.push_back(sprites->Get(SPRITE_BACKGROUND_ID - i));
	}

	vy = -0.07;
}

void CTitleBackGround::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	y += dy;

	// Begin scene 1
	if (!isScene1End && y < -CGame::GetInstance()->GetScreenHeight() / 2)
	{
		isScene1Begin = true;
	}
	// When curtain reach top, end scene 1
	if (isScene1Begin && y < -CGame::GetInstance()->GetScreenHeight())
	{
		vy = 0;
		isScene1Begin = false;
		isScene1End = true;
		isScene2Begin = true;
	}
	// Scene 3
	if (isScene3Begin && scene3_start == 0)
	{
		vy = 0.2f;
		if (y > 0)
		{
			y = 0;
			vy = 0;
			scene3_start = GetTickCount();
		}
	}
	if (isScene3Begin && scene3_start != 0 && GetTickCount() - scene3_start > 1000)
	{
		isScene3End = true;
	}
}

void CTitleBackGround::Render()
{
	if (!isScene1End)
		scrollCurtain->DrawSprite(x, y);
	if (isScene3Begin && !isScene3End)
		backGrounds.at(0)->DrawSprite(x, y);
	if (isScene3End)
	{
		backGrounds.at(1)->DrawSprite(0, 0);
		num3Ani->Render(134, 114, false, false, true);

		switch (playType)
		{
		case ONE_PLAYER_TYPE:
			onePlayerArrowSpr->DrawSprite(84, 170);
			break;
		case TWO_PLAYER_TYPE:
			twoPlayerArrowSpr->DrawSprite(84, 189);
			break;
		default:
			break;
		}
	}
	ground->DrawSprite(x, 221);
}
