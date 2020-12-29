#include "PlayerInfo.h"
#include "Mario.h"

CPlayerInfo* CPlayerInfo::__instance = NULL;

CPlayerInfo::CPlayerInfo()
{
	life = 4;
	money = 0;
	score = 0;
	form = MARIO_LEVEL_SMALL;
}

CPlayerInfo* CPlayerInfo::GetInstance()
{
	if (__instance == NULL) __instance = new CPlayerInfo();
	return __instance;
}
