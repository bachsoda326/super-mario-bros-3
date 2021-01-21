#include "Brick.h"

CBrick::CBrick()
{
	SetBoundingBox();
}

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	isColLeft = false;
	isColRight = false;

	// Intersect with objs
	for (int i = 0; i < coObjects->size(); i++)
	{
		// Check if col with other bricks
		if (!coObjects->at(i)->isDie && dynamic_cast<CBrick*>(coObjects->at(i)))
		{
			if (this == coObjects->at(i))
				continue;
			if (AABBCheck(this, coObjects->at(i)))
			{
				if (top == coObjects->at(i)->bottom || bottom == coObjects->at(i)->top)
					continue;
				OnIntersect(coObjects->at(i), coObjects);
			}
		}
	}
}

void CBrick::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;

	CGameObject::GetBoundingBox(l, t, r, b);
}

void CBrick::OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs)
{
	if (right == obj->left)
		isColRight = true;
	if (left == obj->right)
		isColLeft = true;
}

void CBrick::SetBoundingBox()
{
	left = x;
	top = y;
	right = x + BRICK_BBOX_WIDTH;
	bottom = BRICK_BBOX_HEIGHT;
}