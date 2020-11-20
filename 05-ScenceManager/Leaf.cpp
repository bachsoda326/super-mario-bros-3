#include "Leaf.h"

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CLeaf::Render()
{
	animation_set->at(2)->Render(x, y);

	RenderBoundingBox();
}

void CLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + LEAF_BBOX_WIDTH;
	b = y + LEAF_BBOX_HEIGHT;

	CGameObject::GetBoundingBox(l, t, r, b);
}
