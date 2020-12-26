#include "Bullet.h"
#include "Box.h"
#include "Mario.h"
#include "PlayScence.h"
#include "PSwitch.h"

CBullet::CBullet()
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(BULLET_ANI_SET);
	SetAnimationSet(ani_set);

	isDie = true;
	isDead = true;

	SetBoundingBox();
}

void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;
	
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float distanceX = abs((x + BULLET_BBOX_WIDTH / 2) - (mario->x + MARIO_BIG_BBOX_WIDTH / 2));
	float distanceY = ((y + BULLET_BBOX_HEIGHT / 2) - (mario->y + MARIO_BIG_BBOX_HEIGHT / 2));

	if (x < 0 || x > RIGHT_MAP_1_1 || y < 0 || y > HEIGHT_MAP_1_1 + HEIGHT_UNDER_MAP_1_1 || distanceX > SCREEN_WIDTH || distanceY > SCREEN_HEIGHT)
	{
		isDie = true;
		isDead = true;
	}

	/*if (isEnemy)
	{
		x += dx;
		y += dy;
	}
	else*/ if (!isEnemy)
	{
		if (state == BULLET_STATE_EXPLODE)
		{
			if (GetTickCount() - explode_start > 300)
				isDead = true;
			return;
		}
		
		// Simple fall down
		vy += BULLET_GRAVITY * dt;

		// Giao nhau vs obj
		for (int i = 0; i < coObjects->size(); i++)
		{
			if (dynamic_cast<CBullet*>(coObjects->at(i)))
				continue;
			if (AABBCheck(this, coObjects->at(i)))
			{
				OnIntersect(coObjects->at(i), coObjects);
			}
		}

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		CalcPotentialCollisions(coObjects, coEvents);

		// No collision occured, proceed normally
		if (coEvents.size() == 0)
		{
			/*x += dx;
			y += dy;*/
		}
		else
		{
			float min_tx, min_ty, nx = 0, ny;
			float rdx = 0;
			float rdy = 0;

			// TODO: This is a very ugly designed function!!!!
			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
						
			colX = nx;

			/*if (ny < 0)
			{
				y += min_ty * dy + ny * 0.4f;
				vy = -0.2f;
			}*/

			//
			// Collision logic with other objects
			//
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];

				if (ny < 0 && e->obj != NULL && !isEnemy && (dynamic_cast<CGround*>(e->obj) || dynamic_cast<CBox*>(e->obj) || dynamic_cast<CWarpPipe*>(e->obj) || dynamic_cast<CBrick*>(e->obj)))
				{
					if (!(dynamic_cast<CBreakableBrick*>(e->obj) && e->obj->GetState() == BREAKABLE_BRICK_STATE_COIN))
					{
						vy = -0.2f;
						y = e->obj->y - (bottom - top);
					}
				}

				/*if (dynamic_cast<CMario*>(e->obj) || dynamic_cast<CBullet*>(e->obj))
				{
					x += dx;
					y += dy;
				}
				else if (dynamic_cast<CBox*>(e->obj))
				{
					if (e->nx != 0)
						x += dx;
				}
				else*/ /*if (nx != 0 && !dynamic_cast<CBox*>(e->obj) && !dynamic_cast<CMushRoom*>(e->obj) && !dynamic_cast<CLeaf*>(e->obj) && !dynamic_cast<CCoin*>(e->obj))
				{
					if (!(dynamic_cast<CBreakableBrick*>(e->obj) && e->obj->GetState() == BREAKABLE_BRICK_STATE_COIN))
					{
						vx = 0;
						vy = 0;
						SetState(BULLET_STATE_EXPLODE);
					}
				}*/
				//if (dynamic_cast<CGoomba*>(e->obj))
				//{
				//	/*vx = 0;
				//	vy = 0;
				//	SetState(BULLET_STATE_EXPLODE);*/

				//	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				//	if (!goomba->isDie)
				//	{
				//		goomba->vx = -nx * 0.05f;
				//		goomba->vy = -0.2f;
				//		goomba->SetState(GOOMBA_STATE_DIE_REVERSE);
				//	}
				//}
				//else if (dynamic_cast<CParaGoomba*>(e->obj))
				//{
				//	/*vx = 0;
				//	vy = 0;
				//	SetState(BULLET_STATE_EXPLODE);*/

				//	CParaGoomba* para = dynamic_cast<CParaGoomba*>(e->obj);
				//	if (!para->isDie)
				//	{
				//		para->vx = -nx * 0.05f;
				//		para->vy = -0.1f;
				//		para->SetState(PARA_GOOMBA_STATE_DIE_REVERSE);
				//	}
				//}
				//else if (dynamic_cast<CKoopas*>(e->obj))
				//{
				//	/*vx = 0;
				//	vy = 0;
				//	SetState(BULLET_STATE_EXPLODE);*/

				//	CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				//	if (!koopas->isDie)
				//	{
				//		koopas->vx = -nx * 0.07f;
				//		koopas->vy = -0.2f;
				//		koopas->SetState(KOOPAS_STATE_DIE);
				//	}
				//}
				//else if (dynamic_cast<CPiranha*>(e->obj))
				//{
				//	/*vx = 0;
				//	vy = 0;
				//	SetState(BULLET_STATE_EXPLODE);*/

				//	CPiranha* piranha = dynamic_cast<CPiranha*>(e->obj);
				//	if (piranha->state == PIRANHA_STATE_NORMAL)
				//		piranha->SetState(PIRANHA_STATE_DIE);
				//}
			}

			// clean up collision events
			for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		}
	}		
}

void CBullet::Render()
{
	int ani = -1;

	if (state == BULLET_STATE_FIRE) {
		if (vx > 0) ani = BULLET_ANI_FIRE_RIGHT;
		else if (vx <= 0) ani = BULLET_ANI_FIRE_LEFT;
	}
	if (state == BULLET_STATE_EXPLODE) {
		ani = BULLET_ANI_EXPLODE;
	}

	animation_set->at(ani)->Render(x, y, xReverse, yReverse);

	RenderBoundingBox();
}

void CBullet::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{	
	case BULLET_STATE_FIRE:
		isDie = false;
		isDead = false;
		break;
	case BULLET_STATE_EXPLODE:
		vx = 0;
		vy = 0;
		isDie = true;
		explode_start = GetTickCount();
		break;		
	}
}

void CBullet::OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs)
{
	if (!obj->isDie)
	{
		if (colX != 0 && !dynamic_cast<CBox*>(obj) && !dynamic_cast<CMushRoom*>(obj) && !dynamic_cast<CLeaf*>(obj) && !dynamic_cast<CMushRoom*>(obj) && !dynamic_cast<CCoin*>(obj))
		{
			vx = 0;
			vy = 0;
			SetState(BULLET_STATE_EXPLODE);
		}

		if (dynamic_cast<CGoomba*>(obj))
		{
			vx = 0;
			vy = 0;
			SetState(BULLET_STATE_EXPLODE);

			CGoomba* goomba = dynamic_cast<CGoomba*>(obj);
			if (!goomba->isDie)
			{
				goomba->vx = -nx * 0.05f;
				goomba->vy = -0.2f;
				goomba->SetState(GOOMBA_STATE_DIE_REVERSE);
			}
		}
		else if (dynamic_cast<CParaGoomba*>(obj))
		{
			vx = 0;
			vy = 0;
			SetState(BULLET_STATE_EXPLODE);

			CParaGoomba* para = dynamic_cast<CParaGoomba*>(obj);
			if (!para->isDie)
			{
				para->vx = -nx * 0.05f;
				para->vy = -0.1f;
				para->SetState(PARA_GOOMBA_STATE_DIE_REVERSE);
			}
		}
		else if (dynamic_cast<CKoopas*>(obj))
		{
			vx = 0;
			vy = 0;
			SetState(BULLET_STATE_EXPLODE);

			CKoopas* koopas = dynamic_cast<CKoopas*>(obj);
			if (!koopas->isDie)
			{
				koopas->vx = -nx * 0.07f;
				koopas->vy = -0.2f;
				koopas->SetState(KOOPAS_STATE_DIE);
			}
		}
		else if (dynamic_cast<CPiranha*>(obj))
		{
			vx = 0;
			vy = 0;
			SetState(BULLET_STATE_EXPLODE);

			CPiranha* piranha = dynamic_cast<CPiranha*>(obj);
			if (piranha->state == PIRANHA_STATE_NORMAL)
				piranha->SetState(PIRANHA_STATE_DIE);
		}
	}
}

void CBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BULLET_BBOX_WIDTH;
	bottom = y + BULLET_BBOX_HEIGHT;

	CGameObject::GetBoundingBox(left, top, right, bottom);
}

void CBullet::SetBoundingBox()
{
	left = x;
	top = y;
	right = x + BULLET_BBOX_WIDTH;
	bottom = y + BULLET_BBOX_HEIGHT;
}
