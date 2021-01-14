#include "WarpPipe.h"
#include "Piranha.h"
#include "Game.h"

CWarpPipe::CWarpPipe(float x, float y, int r, int b, int type, float tele_x, float tele_y)
{
	mRight = r;
	mBottom = b;
	this->type = type;	
	if (this->type == PIRANHA_NOR || this->type == PIRANHA_FIRE || this->type == PIRANHA_FIRE_RED)
	{
		piranha = new CPiranha(type, x + PIRANHA_BBOX_WIDTH / 2, y);
		//CGame::GetInstance()->GetCurrentScene()->GetBehindObjs()->push_back(piranha);
	}
	else
	{
		this->tele_x = tele_x;
		this->tele_y = tele_y;
	}

	SetBoundingBox();
}

void CWarpPipe::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	if (piranha != NULL)
		piranha->Update(dt, coObjs);
}

void CWarpPipe::Render()
{
	if (piranha != NULL)
		piranha->Render();

	int ani = 0;
	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CWarpPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = mRight;
	b = mBottom;

	CGameObject::GetBoundingBox(l, t, r, b);
}

void CWarpPipe::SetBoundingBox()
{
	left = x;
	top = y;
	right = mRight;
	bottom = mBottom;
}
