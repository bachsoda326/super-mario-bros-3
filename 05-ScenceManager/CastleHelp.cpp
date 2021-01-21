#include "CastleHelp.h"

CCastleHelp::CCastleHelp()
{
	help_start = GetTickCount();
}

void CCastleHelp::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isHelp && GetTickCount() - help_start > CASTLE_HELP_TIME)
	{
		help_start = GetTickCount();
		isHelp = false;
	}
	if (!isHelp && GetTickCount() - help_start > CASTLE_DISAPPEAR_TIME)
	{
		help_start = GetTickCount();
		isHelp = true;
	}
}

void CCastleHelp::Render()
{
	if (isHelp)
		animation_set->at(0)->Render(x, y);

	RenderBoundingBox();
}
