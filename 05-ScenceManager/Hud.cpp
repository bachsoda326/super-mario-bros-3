#include "Hud.h"
#include "Camera.h"
#include "PlayerInfo.h"
#include "PlayScence.h"

CHud::CHud()
{
	font = new CFont();
	world = WORLD_1;

	hud = CSprites::GetInstance()->Get(SPRITE_HUD_ID);
	cardsHud = CSprites::GetInstance()->Get(SPRITE_CARD_HUD_ID);
	background = CSprites::GetInstance()->Get(SPRITE_BBOX_BLACK_ID);

	worldSpr = font->ToSprite(world + '0');

	for (int i = 0; i < MAX_POWER_STACK; i++)
	{
		if (i < MAX_POWER_STACK - 1)
			powerSpr.push_back(CSprites::GetInstance()->Get(SPRITE_POWER_ID));
		else
			powerSpr.push_back(CSprites::GetInstance()->Get(SPRITE_POWER_ARROW_ID));
	}

	for (int i = 0; i < MAX_POWER_STACK; i++)
	{
		if (i < MAX_POWER_STACK - 1)
			filledPowerSpr.push_back(CSprites::GetInstance()->Get(SPRITE_FILLED_POWER_ID));
		else
			filledPowerSpr.push_back(CSprites::GetInstance()->Get(SPRITE_FILLED_POWER_ARROW_ID));
	}
}

void CHud::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//SetPosition(CCamera::GetInstance()->GetPosition().x, CCamera::GetInstance()->GetPosition().y);
	SetPosition(35, 200);

	if (dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene()))
	{
		time += dt;
		timeRemain = START_TIME - time / 1000;
	}

	string lifeStr = to_string(CPlayerInfo::GetInstance()->GetLife());
	while (lifeStr.size() < LIFE_NUM_COUNT)
		lifeStr = "0" + lifeStr;
	lifeSpr = font->ToSprites(lifeStr);

	string moneyStr = to_string(CPlayerInfo::GetInstance()->GetMoney());	
	moneySpr = font->ToSprites(moneyStr);

	string scoresStr = to_string(CPlayerInfo::GetInstance()->GetScore());
	while (scoresStr.size() < SCORES_NUM_COUNT)
		scoresStr = "0" + scoresStr;
	scoresSpr = font->ToSprites(scoresStr);

	string timeStr = to_string(timeRemain);
	while (timeStr.size() < TIME_NUM_COUNT)
		timeStr = "0" + timeStr;
	timeSpr = font->ToSprites(timeStr);

	/*CPlayerInfo::GetInstance()->GetCards();
	cardsSpr.push_back(CSprites::GetInstance()->Get(SPRITE_POWER_ID));*/
}

void CHud::Render()
{
	background->DrawSprite(0, y);
	hud->DrawSprite(x, y);
	cardsHud->DrawSprite(x + 160, y);
	worldSpr->DrawSprite(x, y);

	/*for (int i = 0; i < lifeSpr.size(); i++)
	{
		lifeSpr[i]->Draw(x, y);
	}*/
}
