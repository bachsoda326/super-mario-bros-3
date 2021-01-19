#include <d3dx9.h>
#include <algorithm>


#include "Utils.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"
#include "Point.h"
#include "PlayerInfo.h"
#include "PlayScence.h"

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;
}

void CGameObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;
}

/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CGameObject::SweptAABBEx(LPGAMEOBJECT coO)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb);

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	coO->GetSpeed(svx, svy);

	float sdx = svx * dt;
	float sdy = svy * dt;

	// (rdx, rdy) is RELATIVE movement distance/velocity 
	float rdx = this->dx - sdx;
	float rdy = this->dy - sdy;

	GetBoundingBox(ml, mt, mr, mb);

	CGame::SweptAABB(
		ml, mt, mr, mb,
		rdx, rdy,
		sl, st, sr, sb,
		t, nx, ny
	);

	CCollisionEvent* e = new CCollisionEvent(t, nx, ny, rdx, rdy, coO);
	return e;
}

bool CGameObject::AABBCheck(CGameObject* obj1, CGameObject* obj2)
{
	float left1, top1, right1, bottom1;
	float left2, top2, right2, bottom2;

	obj1->GetBoundingBox(left1, top1, right1, bottom1);
	obj2->GetBoundingBox(left2, top2, right2, bottom2);

	if (obj1->state == MARIO_STATE_TAIL)
	{
		left1 -= 9;
		right1 += 9;
	}

	// return true nếu collision xảy ra
	return CGame::GetInstance()->AABBCheck(left1, top1, right1, bottom1, left2, top2, right2, bottom2);
}

//bool CGameObject::AABBCheck(float left1, float top1, float right1, float bottom1, float left2, float top2, float right2, float bottom2)
//{	
//	// return true nếu collision xảy ra
//	return !(right1 < left2 || left1 > right2 || bottom1 < top2 || top1 > bottom2);
//}

void CGameObject::ExceptionalCase(CGameObject* obj2, LPCOLLISIONEVENT& coEvent)
{
	if (right > obj2->left && left < obj2->right)
	{
		//Should check again with condition >= top && <= top + i
		if ((int)bottom >= (int)obj2->top && (int)bottom <= (int)obj2->top + 4 && vy > 0)
		{
			coEvent->t = 0.0f;
			coEvent->ny = -1.0f;
			coEvent->nx = 0.0f;
			return;
		}
	}
	//TH đứng trên 2 brick
	if (coEvent->nx != 0 && (right < obj2->left || left > obj2->right) && bottom == obj2->top)
		coEvent->nx = 0.0f;
}

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void CGameObject::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		ExceptionalCase(coObjects->at(i), e);

		if (e->t >= 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CGameObject::FilterCollision(
	vector<LPCOLLISIONEVENT>& coEvents,
	vector<LPCOLLISIONEVENT>& coEventsResult,
	float& min_tx, float& min_ty,
	float& nx, float& ny, float& rdx, float& rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
		}

		if (c->t < min_ty && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}

void CGameObject::PreventMoveX(float nx, LPGAMEOBJECT obj2)
{
	if (nx < 0)						//obj1 va chạm phía bên TRÁI obj2, cạnh trái obj2 - cạnh phải obj1 <= 0 
		x += obj2->left - right;
	else if (nx > 0)				//obj1 va chạm phía bên PHẢI obj2, cạnh phải obj2 - cạnh trái obj1 >= 0 
		x += obj2->right - left;
}

void CGameObject::PreventMoveY(LPGAMEOBJECT obj2)
{
	vy = 0;
	y = obj2->y - (bottom - top);
}

void CGameObject::DeleteObjs(vector<LPGAMEOBJECT>* coObjects)
{
	isDead = true;
	vector<LPGAMEOBJECT>* objs;
	CGrid* grid;
	objs = CGame::GetInstance()->GetCurrentScene()->GetObjs();
	grid = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetGrid();

	for (int i = 0; i < this->listCellIndex.size(); i++)
	{
		int cellIndex = this->listCellIndex[i];
		for (int k = 0; k < grid->listCells[cellIndex].listObj.size(); k++)
		{
			if (grid->listCells[cellIndex].listObj[k] == this)
			{
				grid->listCells[cellIndex].listObj.erase(grid->listCells[cellIndex].listObj.begin() + k);
				break;
			}
		}
	}
	objs->erase(std::remove(objs->begin(), objs->end(), this), objs->end());
	coObjects->erase(std::remove(coObjects->begin(), coObjects->end(), this), coObjects->end());
	delete this;
}

void CGameObject::DeleteFrontObjs(vector<LPGAMEOBJECT>* coObjects)
{
	isDead = true;
	vector<LPGAMEOBJECT>* frontObjs;
	frontObjs = CGame::GetInstance()->GetCurrentScene()->GetFrontObjs();
	frontObjs->erase(std::remove(frontObjs->begin(), frontObjs->end(), this), frontObjs->end());
	coObjects->erase(std::remove(coObjects->begin(), coObjects->end(), this), coObjects->end());
	delete this;
}

void CGameObject::DeleteBehindObjs(vector<LPGAMEOBJECT>* coObjects, bool isDeleteThis)
{
	//isDead = true;
	vector<LPGAMEOBJECT>* behindObjs;
	behindObjs = CGame::GetInstance()->GetCurrentScene()->GetBehindObjs();
	behindObjs->erase(std::remove(behindObjs->begin(), behindObjs->end(), this), behindObjs->end());
	coObjects->erase(std::remove(coObjects->begin(), coObjects->end(), this), coObjects->end());
	if (isDeleteThis) 
	{
		isDead = true;
		delete this; 
	}
}

void CGameObject::AddPoint(int types)
{
	int score = 0;
	int type = types;

	if (type == -1)
	{
		int scoreFactor = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer()->GetPointFactor();
		switch (scoreFactor)
		{
		case 0:
			score = 100;
			type = POINT_100;
			break;
		case 1:
			score = 200;
			type = POINT_200;
			break;
		case 2:
			score = 400;
			type = POINT_400;
			break;
		case 3:
			score = 800;
			type = POINT_800;
			break;
		case 4:
			score = 1000;
			type = POINT_1000;
			break;
		case 5:
			score = 2000;
			type = POINT_2000;
			break;
		case 6:
			score = 4000;
			type = POINT_4000;
			break;
		case 7:
			score = 8000;
			type = POINT_8000;
			break;
			/*case POINT_1_UP:
				score = 1000;
				type = POINT_1_UP;
				break;*/
		default:
			break;
		}
	}
	else
	{
		switch (type)
		{
		case POINT_100:
			score = 100;
			break;
		case POINT_200:
			score = 200;
			break;
		case POINT_400:
			score = 400;
			break;
		case POINT_800:
			score = 800;
			break;
		case POINT_1000:
			score = 1000;
			break;
		case POINT_2000:
			score = 2000;
			break;
		case POINT_4000:
			score = 4000;
			break;
		case POINT_8000:
			score = 8000;
			break;
		case POINT_1_UP:
			CPlayerInfo::GetInstance()->AdjustLife(1);
			break;
		default:
			break;
		}
	}

	CPoint* point = new CPoint(x, y - bottom + top, type);
	CGame::GetInstance()->GetCurrentScene()->GetFrontObjs()->push_back(point);
	CPlayerInfo::GetInstance()->AdjustScore(score);
	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer()->AddPointFactor();
}

void CGameObject::UpdateDirection()
{
	if (vx >= 0)
		nx = 1;
	else
		nx = -1;
}

void CGameObject::Dead()
{
	isDie = true;
	isDead = true;
}


void CGameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	CGame::GetInstance()->Draw(l, t, bbox, rect.left, rect.top, rect.right, rect.bottom, 0, 0, xReverse, yReverse, 128);
}


CGameObject::~CGameObject()
{

}