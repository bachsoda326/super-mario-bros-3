#include "Hud.h"
#include "Camera.h"
#include "PlayerInfo.h"
#include "PlayScence.h"

CHud::CHud()
{
	/*CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(HUD_ANI_SET);
	SetAnimationSet(ani_set);*/
	CPlayerInfo* info = CPlayerInfo::GetInstance();

	font = new CFont();
	world = WORLD_1;

	hud = CSprites::GetInstance()->Get(SPRITE_HUD_ID);
	cardsHud = CSprites::GetInstance()->Get(SPRITE_CARD_HUD_ID);
	background = CSprites::GetInstance()->Get(SPRITE_BBOX_BLACK_ID);
	iconSpr = CSprites::GetInstance()->Get(SPRITE_MARIO_PLAYER_ID);
	worldSpr = font->ToSprite(world + '0');
	filledPowerArrowAni = CAnimations::GetInstance()->Get(ANIMATION_FILLED_POWER_ARROW_ID);

	if (CGame::GetInstance()->GetCurrentScene()->GetSceneId() == MAP_1_1)
		mario = ((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->GetPlayer();

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

	if (info->GetCards().size() == 3)
	{
		info->ClearCard();
	}

	for (int i : info->GetCards())
	{
		cardsSpr.push_back(CSprites::GetInstance()->Get(i));
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

	if (mario != NULL)
	{
		power = mario->GetPower();
	}

	string lifeStr = to_string(CPlayerInfo::GetInstance()->GetLife());
	/*while (lifeStr.size() < LIFE_NUM_COUNT)
		lifeStr = "0" + lifeStr;*/
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
	worldSpr->DrawSprite(x + 36, y + 7);
	iconSpr->DrawSprite(x + 4, y + 15);

	for (int i = lifeSpr.size() - 1; i >= 0; i--)
	{
		lifeSpr[i]->DrawSprite(x + 44 - (lifeSpr.size() - i) * 8, y + 15);
	}

	for (int i = 0; i < timeSpr.size(); i++)
	{
		timeSpr[i]->DrawSprite(x + 124 + i * 8, y + 15);
	}

	for (int i = moneySpr.size() - 1; i >= 0; i--)
	{
		moneySpr[i]->DrawSprite(x + 148 - (moneySpr.size() - i) * 8, y + 7);
	}

	for (int i = 0; i < scoresSpr.size(); i++)
	{
		scoresSpr[i]->DrawSprite(x + 52 + i * 8, y + 15);
	}

	for (int i = 0; i < powerSpr.size(); i++)
	{
		if (i < powerSpr.size() - 1)
			powerSpr[i]->DrawSprite(x + 52 + i * 8, y + 7);
		else
			powerSpr[i]->DrawSprite(x + 53 + i * 8, y + 7);
	}

	for (int i = 0; i < power; i++)
	{
		if (i < filledPowerSpr.size() - 1)
			filledPowerSpr[i]->DrawSprite(x + 52 + i * 8, y + 7);
		else
			/*filledPowerSpr[i]->DrawSprite(x + 53 + i * 8, y + 7);*/
			filledPowerArrowAni->Render(x + 53 + i * 8, y + 7, xReverse, yReverse, true);
	}

	for (int i = 0; i < cardsSpr.size(); i++)
	{
		cardsSpr[i]->DrawSprite(x + 161 + i * 24, y + 2);
	}

	if (cardsAni != NULL)
		cardsAni->Render(x + 161 + (CPlayerInfo::GetInstance()->GetCards().size() - 1) * 24, y + 2, xReverse, yReverse, true);
}

void CHud::EndScene()
{
	int idCard = CPlayerInfo::GetInstance()->GetCards().back();
	int ani = -1;
	switch (idCard)
	{
	case SPRITE_CARD_MUSHROOM_ID:
		ani = ANIMATION_MUSHROOM_CARD_ID;
		break;
	case SPRITE_CARD_STAR_ID:
		ani = ANIMATION_STAR_CARD_ID;
		break;
	case SPRITE_CARD_FLOWER_ID:
		ani = ANIMATION_FLOWER_CARD_ID;
		break;
	default:
		return;
	}
	cardsAni = CAnimations::GetInstance()->Get(ani);
}
