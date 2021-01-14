#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Sprites.h"
#include "Animations.h"
#include "Utils.h"
#include "Constants.h"

using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

class CGameObject; 
typedef CGameObject * LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	
	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL) 
	{ 
		this->t = t; 
		this->nx = nx; 
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj; 
	}

	static bool compare(const LPCOLLISIONEVENT &a, LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};


class CGameObject
{
public:
	int id;

	vector<int> listCellIndex;

	// Respawn obj
	float xSpawn;
	float ySpawn;
	float x; 
	float y;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vxSpawn;
	float vx;
	float vy;

	float left, top, right, bottom;

	int nx;
	int colX, colY;

	int state;
	bool isActive = true;
	bool canActive = true;
	bool isDie = false;
	bool isDead = false;
	bool xReverse = false;
	bool yReverse = false;

	DWORD dt; 

	LPANIMATION_SET animation_set;

	void SetBoundingBox() {};

public: 
	void SetId(int id) { this->id = id; }
	void SetRespawnPosition(float x, float y) { this->xSpawn = x, this->ySpawn = y; }
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }

	int GetState() { return this->state; }

	void RenderBoundingBox();

	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	bool AABBCheck(CGameObject* obj1, CGameObject* obj2);
	//bool AABBCheck(float left1, float top1, float right1, float bottom1, float left2, float top2, float right2, float bottom2);
	void ExceptionalCase(CGameObject* obj2, LPCOLLISIONEVENT& coEvent);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents, 
		vector<LPCOLLISIONEVENT> &coEventsResult, 
		float &min_tx, 
		float &min_ty, 
		float &nx, 
		float &ny, 
		float &rdx, 
		float &rdy);
	void PreventMoveX(float nx, LPGAMEOBJECT obj2);
	void PreventMoveY(LPGAMEOBJECT obj2);

	void DeleteObjs(vector<LPGAMEOBJECT>* coObjects);
	void DeleteFrontObjs(vector<LPGAMEOBJECT>* coObjects);
	void DeleteBehindObjs(vector<LPGAMEOBJECT>* coObjects);
	void AddPoint(int type = -1);

	void UpdateDirection();
	void Dead();

	CGameObject();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) 
	{
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }

	~CGameObject();
};

