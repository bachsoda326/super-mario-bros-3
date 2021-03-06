﻿#pragma once
#include "GameObject.h"
#include "Goomba.h"
#include "MushRoom.h"
#include "Leaf.h"
#include "Bullet.h"
#include "QuestionBrick.h"
#include "Koopas.h"
#include "Coin.h"
// State
#define MARIO_STATE_IDLE					0
#define MARIO_STATE_WALKING					100
#define MARIO_STATE_JUMP_HIGH				101
#define MARIO_STATE_JUMP_SHORT				102
#define MARIO_STATE_PREPARE_RUN				103
#define MARIO_STATE_RUN						104
#define MARIO_STATE_RUNJUMP					105
#define MARIO_STATE_FLY						106
#define MARIO_STATE_KICK					107
#define MARIO_STATE_SKID					108
#define MARIO_STATE_DUCK					109
#define MARIO_STATE_WAG						110
#define MARIO_STATE_IDLE_HOLD				111
#define MARIO_STATE_TAIL					112
#define MARIO_STATE_SHOT					113
#define MARIO_STATE_JUMP_SHOT				114
#define MARIO_STATE_RUNJUMP_SHOT			115
#define MARIO_STATE_EAT_ITEM				116
#define MARIO_STATE_PIPE					117
#define MARIO_STATE_END_SCENE				118
#define MARIO_STATE_BONK					119
#define MARIO_STATE_LOOKUP					120
#define MARIO_STATE_DIE						999

// Animation Small
#define MARIO_ANI_TRANSFORM					154
#define MARIO_ANI_SMALL_IDLE_LEFT			0
#define MARIO_ANI_SMALL_IDLE_RIGHT			1
#define MARIO_ANI_SMALL_WALKING_LEFT		2
#define MARIO_ANI_SMALL_WALKING_RIGHT		3
#define MARIO_ANI_SMALL_SLIDE_LEFT			4
#define MARIO_ANI_SMALL_SLIDE_RIGHT			5
#define MARIO_ANI_SMALL_JUMP_LEFT			6
#define MARIO_ANI_SMALL_JUMP_RIGHT			7
#define MARIO_ANI_SMALL_SKID_LEFT			8
#define MARIO_ANI_SMALL_SKID_RIGHT			9
#define MARIO_ANI_SMALL_RUN_LEFT			10
#define MARIO_ANI_SMALL_RUN_RIGHT			11
#define MARIO_ANI_SMALL_RUNJUMP_LEFT		12
#define MARIO_ANI_SMALL_RUNJUMP_RIGHT		13
#define MARIO_ANI_SMALL_HOLD_LEFT			14
#define MARIO_ANI_SMALL_HOLD_RIGHT			15
#define MARIO_ANI_SMALL_KICK_LEFT			16
#define MARIO_ANI_SMALL_KICK_RIGHT			17
#define MARIO_ANI_SMALL_SWIM_LEFT			18
#define MARIO_ANI_SMALL_SWIM_RIGHT			19
#define MARIO_ANI_SMALL_SWIMINPUT_LEFT		20
#define MARIO_ANI_SMALL_SWIMINPUT_RIGHT		21
#define MARIO_ANI_SMALL_CLIMB_LEFT			22
#define MARIO_ANI_SMALL_CLIMB_RIGHT			23
#define MARIO_ANI_SMALL_PIPE				24
#define MARIO_ANI_SMALL_DIE					25
#define MARIO_ANI_SMALL_PREPARE_RUN_LEFT	138
#define MARIO_ANI_SMALL_PREPARE_RUN_RIGHT	139
#define MARIO_ANI_SMALL_IDLE_HOLD_LEFT		144
#define MARIO_ANI_SMALL_IDLE_HOLD_RIGHT		145
#define MARIO_ANI_SMALL_JUMP_HOLD_LEFT		155
#define MARIO_ANI_SMALL_JUMP_HOLD_RIGHT		156
#define MARIO_ANI_SMALL_RUN_HOLD_LEFT		157
#define MARIO_ANI_SMALL_RUN_HOLD_RIGHT		158

// Animation Big
#define MARIO_ANI_BIG_IDLE_LEFT				26
#define MARIO_ANI_BIG_IDLE_RIGHT			27
#define MARIO_ANI_BIG_WALKING_LEFT			28
#define MARIO_ANI_BIG_WALKING_RIGHT			29
#define MARIO_ANI_BIG_DUCK_LEFT				30
#define MARIO_ANI_BIG_DUCK_RIGHT			31
#define MARIO_ANI_BIG_SLIDE_LEFT			32
#define MARIO_ANI_BIG_SLIDE_RIGHT			33
#define MARIO_ANI_BIG_JUMP_LEFT				34
#define MARIO_ANI_BIG_JUMP_RIGHT			35
#define MARIO_ANI_BIG_SKID_LEFT				36
#define MARIO_ANI_BIG_SKID_RIGHT			37
#define MARIO_ANI_BIG_THROW_LEFT			38
#define MARIO_ANI_BIG_THROW_RIGHT			39
#define MARIO_ANI_BIG_RUN_LEFT				40
#define MARIO_ANI_BIG_RUN_RIGHT				41
#define MARIO_ANI_BIG_RUNJUMP_LEFT			42
#define MARIO_ANI_BIG_RUNJUMP_RIGHT			43
#define MARIO_ANI_BIG_HOLD_LEFT				44
#define MARIO_ANI_BIG_HOLD_RIGHT			45
#define MARIO_ANI_BIG_KICK_LEFT				46
#define MARIO_ANI_BIG_KICK_RIGHT			47
#define MARIO_ANI_BIG_SWIM_LEFT				48
#define MARIO_ANI_BIG_SWIM_RIGHT			49
#define MARIO_ANI_BIG_SWIMINPUT_LEFT		50
#define MARIO_ANI_BIG_SWIMINPUT_RIGHT		51
#define MARIO_ANI_BIG_CLIMB_LEFT			52
#define MARIO_ANI_BIG_CLIMB_RIGHT			53
#define MARIO_ANI_BIG_PIPE					54
#define MARIO_ANI_DOOR_PIPE					55
#define MARIO_ANI_BIG_SPIN_LEFT				56
#define MARIO_ANI_BIG_SPIN_RIGHT			57
#define MARIO_ANI_BIG_BONK_LEFT				58
#define MARIO_ANI_BIG_BONK_RIGHT			59
#define MARIO_ANI_BIG_LOOKUP_LEFT			60
#define MARIO_ANI_BIG_LOOKUP_RIGHT			61
#define MARIO_ANI_BIG_GROW_LEFT				62
#define MARIO_ANI_BIG_GROW_RIGHT			63
#define MARIO_ANI_BIG_SHRINK_LEFT			64
#define MARIO_ANI_BIG_SHRINK_RIGHT			65
#define MARIO_ANI_BIG_PREPARE_RUN_LEFT		134
#define MARIO_ANI_BIG_PREPARE_RUN_RIGHT		135
#define MARIO_ANI_BIG_IDLE_HOLD_LEFT		146
#define MARIO_ANI_BIG_IDLE_HOLD_RIGHT		147
#define MARIO_ANI_BIG_JUMP_HOLD_LEFT		159
#define MARIO_ANI_BIG_JUMP_HOLD_RIGHT		160
#define MARIO_ANI_BIG_RUN_HOLD_LEFT			161
#define MARIO_ANI_BIG_RUN_HOLD_RIGHT		162

// Animation Fire
#define MARIO_ANI_FIRE_IDLE_LEFT			66
#define MARIO_ANI_FIRE_IDLE_RIGHT			67
#define MARIO_ANI_FIRE_WALKING_LEFT			68
#define MARIO_ANI_FIRE_WALKING_RIGHT		69
#define MARIO_ANI_FIRE_DUCK_LEFT			70
#define MARIO_ANI_FIRE_DUCK_RIGHT			71
#define MARIO_ANI_FIRE_SLIDE_LEFT			72
#define MARIO_ANI_FIRE_SLIDE_RIGHT			73
#define MARIO_ANI_FIRE_JUMP_LEFT			74
#define MARIO_ANI_FIRE_JUMP_RIGHT			75
#define MARIO_ANI_FIRE_SKID_LEFT			76
#define MARIO_ANI_FIRE_SKID_RIGHT			77
#define MARIO_ANI_FIRE_THROW_LEFT			78
#define MARIO_ANI_FIRE_THROW_RIGHT			79
#define MARIO_ANI_FIRE_RUN_LEFT				80
#define MARIO_ANI_FIRE_RUN_RIGHT			81
#define MARIO_ANI_FIRE_RUNJUMP_LEFT			82
#define MARIO_ANI_FIRE_RUNJUMP_RIGHT		83
#define MARIO_ANI_FIRE_HOLD_LEFT			84
#define MARIO_ANI_FIRE_HOLD_RIGHT			85
#define MARIO_ANI_FIRE_KICK_LEFT			86
#define MARIO_ANI_FIRE_KICK_RIGHT			87
#define MARIO_ANI_FIRE_SWIMIDLE_LEFT		88
#define MARIO_ANI_FIRE_SWIMIDLE_RIGHT		89
#define MARIO_ANI_FIRE_SWIMINPUT_LEFT		90
#define MARIO_ANI_FIRE_SWIMINPUT_RIGHT		91
#define MARIO_ANI_FIRE_CLIMB_LEFT			92
#define MARIO_ANI_FIRE_CLIMB_RIGHT			93
#define MARIO_ANI_FIRE_PIPE					94
#define MARIO_ANI_FIRE_DOOR					95
#define MARIO_ANI_FIRE_SPIN_LEFT			96
#define MARIO_ANI_FIRE_SPIN_RIGHT			97
#define MARIO_ANI_FIRE_PREPARE_RUN_LEFT		142
#define MARIO_ANI_FIRE_PREPARE_RUN_RIGHT	143
#define MARIO_ANI_FIRE_IDLE_HOLD_LEFT		150
#define MARIO_ANI_FIRE_IDLE_HOLD_RIGHT		151
#define MARIO_ANI_FIRE_JUMP_HOLD_LEFT		167
#define MARIO_ANI_FIRE_JUMP_HOLD_RIGHT		168
#define MARIO_ANI_FIRE_RUN_HOLD_LEFT		169
#define MARIO_ANI_FIRE_RUN_HOLD_RIGHT		170

// Animation Raccoon
#define MARIO_ANI_RACCOON_IDLE_LEFT			98
#define MARIO_ANI_RACCOON_IDLE_RIGHT		99
#define MARIO_ANI_RACCOON_WAG_LEFT			102
#define MARIO_ANI_RACCOON_WAG_RIGHT			103
#define MARIO_ANI_RACCOON_WALKING_LEFT		100
#define MARIO_ANI_RACCOON_WALKING_RIGHT		101
#define MARIO_ANI_RACCOON_DUCK_LEFT			104
#define MARIO_ANI_RACCOON_DUCK_RIGHT		105
#define MARIO_ANI_RACCOON_SLIDE_LEFT		106
#define MARIO_ANI_RACCOON_SLIDE_RIGHT		107
#define MARIO_ANI_RACCOON_JUMP_LEFT			108
#define MARIO_ANI_RACCOON_JUMP_RIGHT		109
#define MARIO_ANI_RACCOON_SKID_LEFT			110
#define MARIO_ANI_RACCOON_SKID_RIGHT		111
#define MARIO_ANI_RACCOON_THROW_LEFT		112
#define MARIO_ANI_RACCOON_THROW_RIGHT		113
#define MARIO_ANI_RACCOON_RUN_LEFT			114
#define MARIO_ANI_RACCOON_RUN_RIGHT			115
#define MARIO_ANI_RACCOON_RUNJUMP_LEFT		116
#define MARIO_ANI_RACCOON_RUNJUMP_RIGHT		117
#define MARIO_ANI_RACCOON_FLY_LEFT			118
#define MARIO_ANI_RACCOON_FLY_RIGHT			119
#define MARIO_ANI_RACCOON_HOLD_LEFT			120
#define MARIO_ANI_RACCOON_HOLD_RIGHT		121
#define MARIO_ANI_RACCOON_KICK_LEFT			122
#define MARIO_ANI_RACCOON_KICK_RIGHT		123
#define MARIO_ANI_RACCOON_SWIMIDLE_LEFT		124
#define MARIO_ANI_RACCOON_SWIMIDLE_RIGHT	125
#define MARIO_ANI_RACCOON_SWIMINPUT_LEFT	126
#define MARIO_ANI_RACCOON_SWIMINPUT_RIGHT	127
#define MARIO_ANI_RACCOON_CLIMB_LEFT		128
#define MARIO_ANI_RACCOON_CLIMB_RIGHT		129
#define MARIO_ANI_RACCOON_PIPE				130
#define MARIO_ANI_RACCOON_DOOR				131
#define MARIO_ANI_RACCOON_SPIN_LEFT			132
#define MARIO_ANI_RACCOON_SPIN_RIGHT		133
#define MARIO_ANI_RACCOON_TAIL_LEFT			136
#define MARIO_ANI_RACCOON_TAIL_RIGHT		137
#define MARIO_ANI_RACCOON_PREPARE_RUN_LEFT	140
#define MARIO_ANI_RACCOON_PREPARE_RUN_RIGHT	141
#define MARIO_ANI_RACCOON_IDLE_HOLD_LEFT	148
#define MARIO_ANI_RACCOON_IDLE_HOLD_RIGHT	149
#define MARIO_ANI_RACCOON_JUMP_HOLD_LEFT	163
#define MARIO_ANI_RACCOON_JUMP_HOLD_RIGHT	164
#define MARIO_ANI_RACCOON_RUN_HOLD_LEFT		165
#define MARIO_ANI_RACCOON_RUN_HOLD_RIGHT	166

// Level
#define	MARIO_LEVEL_SMALL					1
#define	MARIO_LEVEL_BIG						2
#define	MARIO_LEVEL_RACCOON					3
#define	MARIO_LEVEL_FIRE					4
// BBox
#define MARIO_TAIL_BBOX_WIDTH				14
#define MARIO_NON_TAIL_BBOX_HEIGHT			16
#define MARIO_BIG_BBOX_WIDTH				15
#define MARIO_BIG_BBOX_HEIGHT				27
#define MARIO_BIG_BBOX_HEIGHT_DUCK			18
#define MARIO_SMALL_BBOX_WIDTH				16
#define MARIO_SMALL_BBOX_HEIGHT				16
#define MARIO_PIPE_DOWN_HEIGHT				7
// Time
#define MARIO_UNTOUCHABLE_TIME				3500
#define MARIO_RUN_TIME						3000
#define MARIO_DIE_TIME						3000
#define MARIO_TRANSFORM_TIME				500
#define MARIO_KICK_TIME						100
#define MARIO_TAIL_TIME						210
#define MARIO_FIRE_BULLET_TIME				150
#define MARIO_WAG_FLY_TIME					300
#define MARIO_FLY_LIMIT_TIME				5000
#define MARIO_PIPE_DOWN_TIME				1000
#define MARIO_PIPE_UP_TIME					1500
#define MARIO_PIPE_UP_TIME_2				2500
#define MARIO_PIPE_UP_TIME_3				1000
#define MARIO_EAT_ITEM_TIME					1
#define MARIO_POWER_INCREASE_TIME			170
#define MARIO_POWER_DECREASE_TIME			300
// Speed
#define MARIO_GRAVITY						0.001f
#define MARIO_TITLE_SCENE_GRAVITY			0.0005f
#define MARIO_WALKING_SPEED					0.1f 
#define MARIO_PREPARE_RUN_SPEED				0.15f
#define MARIO_RUN_SPEED						0.2f 
#define MARIO_JUMP_HIGH_SPEED_Y				0.02f
#define MARIO_JUMP_SHORT_SPEED_Y			0.27f
#define MARIO_JUMP_DEFLECT_SPEED			0.2f
#define MARIO_DIE_DEFLECT_SPEED				0.5f
#define MARIO_MAX_Y_SPEED					0.2f
#define MARIO_JUMP_LUGI_Y_SPEED				0.42f
#define MARIO_WAG_Y_SPEED					0.05f
#define MARIO_FLY_Y_SPEED					0.08f
#define MARIO_PIPE_Y_SPEED					0.02f
#define MARIO_DEFLECT_SPEED					0.4f
#define MARIO_JUMP_Y_SPEED					0.15f
#define MARIO_INCREASE_X_SPEED				0.005f
#define MARIO_DECREASE_X_SPEED				0.004f
#define MARIO_CAN_SKID_X_SPEED				0.07f
#define MARIO_INCREASE_RUN_X_SPEED			0.0015f
// Others
#define MARIO_DEFLECT_TAIL_TOP				17
#define MARIO_DEFLECT_TAIL_BOTTOM			2
#define MARIO_DEFLECT_TAIL_WIDTH			9
#define MARIO_NUM_BULLETS					2


class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;
	bool isEndScene = false;

	int power = 0;
	int pointFactor = 0;

	float start_x;			// initial position of Mario at scene
	float start_y;
	float pipe_tele_x;
	float pipe_tele_y;

	void SetBoundingBox();
public:
	DWORD transform_start;
	DWORD run_start;
	DWORD kick_start;
	DWORD skid_start;
	DWORD tail_start;
	DWORD fire_start;
	DWORD wag_start;
	DWORD fly_start;
	DWORD fly_limit_start;
	DWORD eat_item_start;
	DWORD pipe_up_start;
	DWORD pipe_down_start;
	DWORD die_start;
	DWORD canJump_start;
	// For Title Scene
	DWORD duck_start;
	DWORD bonk_start;
	DWORD look_start;

	/*bool isPreventMoveX = false;*/
	bool isTransform = false;
	bool isOnGround = false;
	bool canJump = true;
	bool canRepeatJump = true;
	bool canJumpHigher = true;
	bool canHold = false;
	bool isHold = false;
	bool canAttack = true;
	bool canHit = true;
	bool canMultiScoreLand = false;
	bool canMultiScoreJump = false;
	bool isToPipeDown = true;
	bool isOnOtherMap = false;
	bool isHitGoomba = false;
	bool isKickKoopas = false;
	//bool canDuck = false;

	vector<CBullet*>* bullets;
	CKoopas* koopas;

	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	int GetLevel() { return level; }
	int GetPower() { return power; }
	int GetPointFactor() { return pointFactor; }
	bool IsEndScene() { return isEndScene; }

	void AddPointFactor();
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void SetKoopas(CKoopas* koopas) { this->koopas = koopas; };
	void Reset();

	void Hurt();
	void IncreasePower();
	void DecreasePower();
	void EatMushRoom(CMushRoom* mushroom, vector<LPGAMEOBJECT>* coObjs);
	void EatLeaf(CLeaf* leaf, vector<LPGAMEOBJECT>* coObjs);
	void EatCoin(CCoin* coin, vector<LPGAMEOBJECT>* coObjs);

	// Intersert with objs
	void OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs);
	bool isColTail(CGameObject* obj);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};