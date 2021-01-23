﻿#include "WorldMapScene.h"
#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "TileMap.h"
#include "Camera.h"

using namespace std;

CWorldMapScene::CWorldMapScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CWorldMapSceneKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN			-1
#define SCENE_SECTION_TEXTURES			2
#define SCENE_SECTION_SPRITES			3
#define SCENE_SECTION_ANIMATIONS		4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS			6
#define SCENE_SECTION_MAP				7

#define OBJECT_TYPE_MARIO				0
#define OBJECT_TYPE_CASTLE_HELP			1
#define	OBJECT_TYPE_HAMMER				2
#define OBJECT_TYPE_BUSH				3
#define OBJECT_TYPE_STATION				4

#define OBJECT_TYPE_PORTAL				50
#define OBJECT_TYPE_HUD					51

#define WOLRD_MAP_Y_DISTANCE			10

#define MAX_SCENE_LINE					1024

void CWorldMapScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CWorldMapScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());
	int xD = 0;
	int yD = 0;
	if (tokens.size() > 7)
	{
		xD = atoi(tokens[6].c_str());
		yD = atoi(tokens[7].c_str());
	}

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, xD, yD, tex);
}

void CWorldMapScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CWorldMapScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS]
*/
void CWorldMapScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = LPANIMATION_SET();
	if (ani_set_id != -1)
		ani_set = animation_sets->Get(ani_set_id);

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CWorldMario();
		player = (CWorldMario*)obj;

		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_BUSH:
		obj = new CWorldBush();
		break;
	case OBJECT_TYPE_HAMMER:
	{
		float left = atof(tokens[4].c_str());
		float right = atof(tokens[5].c_str());
		obj = new CWorldHammer(left, right);
	}
	break;
	case OBJECT_TYPE_CASTLE_HELP:
		obj = new CCastleHelp();
		break;
	case OBJECT_TYPE_STATION:
	{
		int id = atoi(tokens[4].c_str());
		bool left = atoi(tokens[5].c_str()) == 1 ? true : false;
		bool top = atoi(tokens[6].c_str()) == 1 ? true : false;
		bool right = atoi(tokens[7].c_str()) == 1 ? true : false;
		bool bottom = atoi(tokens[8].c_str()) == 1 ? true : false;
		obj = new CWorldStation(id, left, top, right, bottom);
	}
	break;
	case OBJECT_TYPE_HUD:
	{
		hud = new CHud();
		break;
	}
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	if (obj != NULL)
	{
		// General object setup
		obj->SetPosition(x, y);

		/*LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);*/

		if (ani_set_id != -1)
			obj->SetAnimationSet(ani_set);

		objects.push_back(obj);
	}
}

void CWorldMapScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;

	int idTexMap = atoi(tokens[1].c_str());
	float mapWidth = atoi(tokens[2].c_str());
	float mapHeight = atoi(tokens[3].c_str());
	map = new CTileMap(idTexMap, tokens[0]);

	switch (map->GetId())
	{
	case WORLD_MAP_1:
		CCamera::GetInstance()->SetMapSize(LEFT_WORLD_MAP_1, TOP_WORLD_MAP_1, RIGHT_WORLD_MAP_1, BOTTOM_WORLD_MAP_1, WIDTH_WORLD_MAP_1, HEIGHT_WORLD_MAP_1);
	default:
		break;
	}
}

void CWorldMapScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[MAP]") {
			section = SCENE_SECTION_MAP; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CWorldMapScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	// add to colliable objs
	vector<LPGAMEOBJECT> coObjects;
	/*for (size_t i = 0; i < otherObjs.size(); i++)
	{
		if (!otherObjs[i]->isDie || !otherObjs[i]->isDead)
			coObjects.push_back(otherObjs[i]);
	}*/
	for (size_t i = 1; i < objects.size(); i++)
	{
		if (!objects[i]->isDie || !objects[i]->isDead)
			coObjects.push_back(objects[i]);
	}

	// update all objs
	/*for (size_t i = 0; i < otherObjs.size(); i++)
	{
		if (!otherObjs[i]->isDead)
			otherObjs[i]->Update(dt, &coObjects);
	}*/
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (!objects[i]->isDead)
			objects[i]->Update(dt, &coObjects);
	}

	hud->Update(dt);

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera

	switch (map->GetId())
	{
	case WORLD_MAP_1:
		CCamera::GetInstance()->SetPosition(WIDTH_WORLD_MAP_1 / 2, HEIGHT_WORLD_MAP_1 / 2 + WOLRD_MAP_Y_DISTANCE);
		//CCamera::GetInstance()->SetPosition(CGame::GetInstance()->GetScreenWidth() / 2, CGame::GetInstance()->GetScreenHeight() / 2);
	default:
		break;
	}

}

void CWorldMapScene::Render()
{
	if (map != NULL)
	{
		map->Render();
	}

	/*for (int i = 0; i < otherObjs.size(); i++)
	{
		if (!otherObjs[i]->isDead)
			otherObjs[i]->Render();
	}*/
	for (int i = 0; i < objects.size(); i++)
	{
		if (!objects[i]->isDead)
			objects[i]->Render();
	}

	hud->Render();
}

/*
	Unload current scene
*/
void CWorldMapScene::Unload()
{
	// other objs
	/*for (int i = 0; i < otherObjs.size(); i++)
		delete otherObjs[i];

	otherObjs.clear();*/

	// objs
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	delete map;
	delete hud;
	map = NULL;
	hud = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CWorldMapSceneKeyHandler::OnKeyDown(int KeyCode)
{
	CWorldMario* mario = ((CWorldMapScene*)scence)->GetPlayer();

	switch (KeyCode)
	{
	case DIK_C:
		CGame::GetInstance()->SwitchScene(MAP_1_1);
		break;
	case DIK_V:
		CGame::GetInstance()->SwitchScene(TITLE_SCREEN);
		break;
	case DIK_LEFT:
		mario->GoLeft();
		break;
	case DIK_UP:
		mario->GoTop();
		break;
	case DIK_RIGHT:
		mario->GoRight();
		break;
	case DIK_DOWN:
		mario->GoBottom();
		break;
	case DIK_S:
	{
		int sceneId = mario->GetCurrentStation()->GetSceneId();
		if (sceneId == MAP_1_1 || sceneId == MAP_1_4 || sceneId == WORLD_MAP_1)
		{
			if (sceneId == MAP_1_4)
				CCamera::GetInstance()->SetIsMoving(true);
			CGame::GetInstance()->SwitchScene(sceneId);
		}
	}
	break;
	default:
		break;
	}
}

void CWorldMapSceneKeyHandler::OnKeyUp(int KeyCode)
{
	///*DebugOut(L"[KEYUP] KeyUp: %d\n", KeyCode);*/

	//CGame* game = CGame::GetInstance();
	//CMario* mario = ((CWorldMapScene*)scence)->GetPlayer();

	//switch (KeyCode)
	//{
	//case DIK_X:
	//	break;
	//case DIK_S:
	//	mario->canJump = false;
	//	mario->canRepeatJump = true;
	//	break;
	//case DIK_A:
	//	mario->canHold = false;

	//	if (mario->isHold)
	//	{
	//		if (mario->koopas != NULL)
	//		{
	//			mario->kick_start = GetTickCount();
	//			mario->koopas->SetState(KOOPAS_STATE_SPIN);
	//			mario->koopas->vx = mario->nx * KOOPAS_SPIN_SPEED;
	//			mario->koopas = NULL;
	//			mario->isHold = false;
	//		}
	//	}
	//	else if (mario->state == MARIO_STATE_PREPARE_RUN || mario->state == MARIO_STATE_RUN)
	//	{
	//		mario->SetState(MARIO_STATE_WALKING);
	//		mario->vx = mario->nx * MARIO_WALKING_SPEED;
	//	}
	//	break;
	//}
}

void CWorldMapSceneKeyHandler::KeyState(BYTE* states)
{
	////DebugOut(L"[KEYSTATE] KeyState: %d\n", states);

	//CGame* game = CGame::GetInstance();
	//CMario* mario = ((CWorldMapScene*)scence)->GetPlayer();
	//int state = mario->GetState();
	//// disable control key when Mario die 
	//if (state == MARIO_STATE_DIE) return;
	//if (game->IsKeyDown(DIK_RIGHT) && state != MARIO_STATE_PIPE)
	//{
	//	mario->nx = 1;
	//	if (state != MARIO_STATE_DUCK /*&& !mario->isPreventMoveX*/)
	//		if (mario->vx < MARIO_WALKING_SPEED)
	//			mario->vx += 0.005f;

	//	// Phanh
	//	if (mario->vx < 0 && mario->vx < -0.07f && mario->isOnGround && !mario->isHold)
	//		mario->SetState(MARIO_STATE_SKID);
	//	else if (mario->vx >= 0 && mario->vy == 0 && state != MARIO_STATE_PREPARE_RUN && state != MARIO_STATE_RUN)
	//		mario->SetState(MARIO_STATE_WALKING);
	//	if (game->IsKeyDown(DIK_A))
	//	{
	//		if (state != MARIO_STATE_RUN)
	//		{
	//			if (mario->vx < mario->nx * MARIO_PREPARE_RUN_SPEED)
	//				mario->vx += 0.0015f;
	//		}

	//		if (state == MARIO_STATE_WALKING)
	//		{
	//			if (mario->vx >= mario->nx * MARIO_PREPARE_RUN_SPEED)
	//			{
	//				mario->run_start = GetTickCount();
	//				mario->SetState(MARIO_STATE_PREPARE_RUN);
	//			}
	//		}
	//		else if (state == MARIO_STATE_PREPARE_RUN && !mario->isHold && GetTickCount() - mario->run_start >= MARIO_RUN_TIME / 6)
	//		{
	//			mario->SetState(MARIO_STATE_RUN);
	//		}
	//	}
	//	/*else if (mario->vy == 0)
	//	{
	//		mario->SetState(MARIO_STATE_WALKING);
	//		mario->vx = MARIO_WALKING_SPEED;
	//	}*/
	//}
	//else if (game->IsKeyDown(DIK_LEFT) && state != MARIO_STATE_PIPE)
	//{
	//	mario->nx = -1;
	//	if (state != MARIO_STATE_DUCK /*&& !mario->isPreventMoveX*/)
	//		if (mario->vx > -MARIO_WALKING_SPEED)
	//			mario->vx -= 0.005f;

	//	// Phanh
	//	if (mario->vx > 0 && mario->vx > 0.07f && mario->isOnGround && !mario->isHold)
	//		mario->SetState(MARIO_STATE_SKID);
	//	else if (mario->vx <= 0 && mario->vy == 0 && state != MARIO_STATE_PREPARE_RUN && state != MARIO_STATE_RUN)
	//		mario->SetState(MARIO_STATE_WALKING);

	//	if (game->IsKeyDown(DIK_A))
	//	{
	//		/*if (state != MARIO_STATE_RUN)
	//			mario->vx = 1.5 * mario->nx * MARIO_WALKING_SPEED;*/

	//		if (state != MARIO_STATE_RUN)
	//		{
	//			if (mario->vx > mario->nx * MARIO_PREPARE_RUN_SPEED)
	//				mario->vx -= 0.0015f;
	//		}

	//		if (state == MARIO_STATE_WALKING)
	//		{
	//			if (mario->vx <= mario->nx * MARIO_PREPARE_RUN_SPEED)
	//			{
	//				mario->run_start = GetTickCount();
	//				mario->SetState(MARIO_STATE_PREPARE_RUN);
	//			}
	//		}
	//		else if (state == MARIO_STATE_PREPARE_RUN && !mario->isHold && GetTickCount() - mario->run_start >= MARIO_RUN_TIME / 3)
	//		{
	//			mario->SetState(MARIO_STATE_RUN);
	//		}
	//	}
	//	/*else if (mario->vy == 0)
	//		mario->SetState(MARIO_STATE_WALKING);*/
	//}
	///*else if (!mario->isOnGround)
	//{
	//	mario->SetState(MARIO_STATE_JUMP_HIGH);
	//}*/
	//else if (mario->isOnGround)
	//{
	//	if (mario->nx > 0)
	//	{
	//		mario->SetState(MARIO_STATE_WALKING);
	//		mario->vx -= 0.004f;
	//		if (mario->vx <= 0)
	//			mario->SetState(MARIO_STATE_IDLE);
	//	}
	//	if (mario->nx < 0)
	//	{
	//		mario->SetState(MARIO_STATE_WALKING);
	//		mario->vx += 0.004f;
	//		if (mario->vx >= 0)
	//			mario->SetState(MARIO_STATE_IDLE);
	//	}
	//	//mario->SetState(MARIO_STATE_IDLE);
	//}
	//if (game->IsKeyDown(DIK_S) && mario->canJump)
	//{
	//	if (mario->canJumpHigher)
	//		mario->vy -= MARIO_JUMP_HIGH_SPEED_Y;
	//	if (abs(mario->vx) == MARIO_RUN_SPEED)
	//		mario->SetState(MARIO_STATE_RUNJUMP);
	//	else
	//		mario->SetState(MARIO_STATE_JUMP_HIGH);
	//}
	//if (game->IsKeyDown(DIK_DOWN) && mario->isOnGround && mario->GetLevel() != MARIO_LEVEL_SMALL && !mario->isHold)
	//{
	//	mario->SetState(MARIO_STATE_DUCK);
	//}
	//if (game->IsKeyDown(DIK_A) && mario->isHold)
	//{
	//	/*mario->SetState(MARIO_STATE_IDLE_HOLD);*/
	//	if (mario->koopas != NULL)
	//	{
	//		float l, t, r, b;
	//		mario->GetBoundingBox(l, t, r, b);

	//		if (mario->nx > 0)
	//		{
	//			if (mario->GetLevel() == MARIO_LEVEL_SMALL)
	//				mario->koopas->SetPosition(r - 4, t - MARIO_SMALL_BBOX_HEIGHT / 4);
	//			else
	//				mario->koopas->SetPosition(r - 4, t + MARIO_BIG_BBOX_HEIGHT / 9);
	//		}
	//		else
	//		{
	//			if (mario->GetLevel() == MARIO_LEVEL_SMALL)
	//				mario->koopas->SetPosition(l - (mario->koopas->right - mario->koopas->left) + 4, t - MARIO_SMALL_BBOX_HEIGHT / 4);
	//			else
	//				mario->koopas->SetPosition(l - (mario->koopas->right - mario->koopas->left) + 4, t + MARIO_BIG_BBOX_HEIGHT / 9);
	//		}
	//	}
	//}
	///*if (game->IsKeyDown(DIK_A))
	//{
	//	if (state != MARIO_STATE_IDLE && mario->vx != 0)
	//	{
	//		mario->vx = 1.5 * mario->nx * MARIO_WALKING_SPEED;

	//		if (state == MARIO_STATE_WALKING)
	//		{
	//			mario->run_start = GetTickCount();
	//			mario->SetState(MARIO_STATE_PREPARE_RUN);
	//		}
	//		else if (state == MARIO_STATE_PREPARE_RUN && GetTickCount() - mario->run_start >= MARIO_RUN_TIME / 3)
	//		{
	//			mario->SetState(MARIO_STATE_RUN);
	//		}
	//	}
	//}*/
}