#include "CastleHelp.h"

CCastleHelp::CCastleHelp()
{
	help_start = GetTickCount();
}

void CCastleHelp::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isHelp && GetTickCount() - help_start > 1000)
	{
		help_start = GetTickCount();
		isHelp = false;
	}
	if (!isHelp && GetTickCount() - help_start > 300)
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
