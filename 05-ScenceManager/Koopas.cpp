#include "Koopas.h"

CKoopas::CKoopas(int type, float min, float max)
{
	switch (type)
	{
	case 1:
		this->type = KOOPAS_GREEN;
		break;
	case 2:
		this->type = KOOPAS_RED;
		break;
	default:
		break;
	}

	if (min != -1 && max != -1)
	{
		xMin = min;
		xMax = max;
	}

	SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_HIDE || state == KOOPAS_STATE_SPIN || state == KOOPAS_STATE_HOLD)
		bottom = y + KOOPAS_BBOX_HEIGHT_HIDE;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	/*x += dx;
	y += dy;

	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > 290) {
		x = 290; vx = -vx;
	}*/

	// Simple fall down
	if (state != KOOPAS_STATE_HOLD)
		vy += KOOPAS_GRAVITY * dt;
	/*else
	{

	}*/

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	/*if (state != KOOPAS_STATE_DIE)*/
	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		/*x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;*/

		if (state == KOOPAS_STATE_HOLD)
		{
			if (nx != 0)
			{
				x += min_tx * dx + nx * 0.4f;
				vx = 0;
			}
		}
		else
			x += dx;
		if (ny < 0) 
		{
			y += min_ty * dy + ny * 0.4f;
			vy = 0; 
		}

		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			/*if (dynamic_cast<CBox*>(e->obj) || dynamic_cast<CGoomba*>(e->obj) || dynamic_cast<CKoopas*>(e->obj))
			{
				x += dx;
			}*/
			if (dynamic_cast<CGoomba*>(e->obj))
			{
				if (state == KOOPAS_STATE_SPIN)
				{
					CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
					if (goomba->state != GOOMBA_STATE_DIE_REVERSE)
					{
						goomba->vx = -nx * 0.1f;
						goomba->vy = -0.2f;
						goomba->SetState(GOOMBA_STATE_DIE_REVERSE);
					}
				}
			}
			if (dynamic_cast<CGround*>(e->obj) || dynamic_cast<CWarpPipe*>(e->obj) || dynamic_cast<CBrick*>(e->obj))
			{
				if (e->nx != 0)
				{
					if (state == KOOPAS_STATE_HOLD)
					{
						/*x += min_tx * dx + nx * 0.4f;
						vx = 0;*/
					}
					else
					{
						x += min_tx * dx + nx * 0.4f;
						vx = -vx;
					}
				}
			}
			if (type == KOOPAS_RED)
			{
				if (state == KOOPAS_STATE_WALKING)
				{
					if (x <= xMin)
					{
						x = xMin;
						vx = -vx;
					}
					if (x >= xMax)
					{
						x = xMax;
						vx = -vx;
					}
				}
			}
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CKoopas::Render()
{
	int ani = KOOPAS_ANI_WALKING_LEFT;
	if (state == KOOPAS_STATE_HIDE || state == KOOPAS_STATE_HOLD) {
		ani = KOOPAS_ANI_HIDE;
	}
	else if (state == KOOPAS_STATE_SPIN) {
		ani = KOOPAS_ANI_SPIN_LEFT;
	}
	else if (vx > 0) ani = KOOPAS_ANI_WALKING_RIGHT;
	else if (vx <= 0) ani = KOOPAS_ANI_WALKING_LEFT;

	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		/*case KOOPAS_STATE_DIE:
			y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_HIDE + 1;
			vx = 0;
			vy = 0;
			break;*/
	case KOOPAS_STATE_HIDE:
		//y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_HIDE + 1;
		vx = 0;
		vy = 0;
		break;
		/*case KOOPAS_STATE_SPIN:
			vx = -KOOPAS_SPIN_SPEED;
			break;*/
	case KOOPAS_STATE_WALKING:
		vx = -KOOPAS_WALKING_SPEED;
		break;
		/*case KOOPAS_STATE_HOLD:
			vx = -KOOPAS_WALKING_SPEED;
			break;*/
	}

}