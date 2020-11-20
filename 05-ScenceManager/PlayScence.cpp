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

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_MAP 7

#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3
#define OBJECT_TYPE_BOX	4
#define OBJECT_TYPE_GROUND	5
#define OBJECT_TYPE_QUESTION_BRICK	6
#define OBJECT_TYPE_WARPPIPE	7
#define OBJECT_TYPE_BREAKABLE_BRICK	9
#define OBJECT_TYPE_MUSHROOM	10
#define OBJECT_TYPE_LEAF	11

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


void CPlayScene::_ParseSection_TEXTURES(string line)
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

void CPlayScene::_ParseSection_SPRITES(string line)
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

void CPlayScene::_ParseSection_ANIMATIONS(string line)
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

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
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
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x, y);
		player = (CMario*)obj;

		for (int i = 0; i < 2; i++)
		{
			CBullet* bullet = new CBullet();
			bullet->SetAnimationSet(ani_set);
			player->bullets->push_back(bullet);
			objects.push_back(player->bullets->at(i));
		}

		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_QUESTION_BRICK: obj = new CQuestionBrick(); break;
	case OBJECT_TYPE_BREAKABLE_BRICK: obj = new CBreakableBrick(); break;
	case OBJECT_TYPE_MUSHROOM: obj = new CMushRoom(); break;
	case OBJECT_TYPE_LEAF: obj = new CLeaf(); break;
	case OBJECT_TYPE_KOOPAS:
	{
		int type = atoi(tokens[4].c_str());
		if (type == 2)
		{
			float min = atof(tokens[5].c_str());
			float max = atof(tokens[6].c_str());
			obj = new CKoopas(type, min, max);
		}
		else
			obj = new CKoopas(type);
	}
	break;
	case OBJECT_TYPE_BOX:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		obj = new CBox(r, b);
	}
	break;
	case OBJECT_TYPE_WARPPIPE:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		obj = new CWarpPipe(r, b);
	}
	break;
	case OBJECT_TYPE_GROUND:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		obj = new CGround(r, b);
	}
	break;
	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	/*LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);*/

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}

void CPlayScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;

	int idTex = atoi(tokens[1].c_str());
	float mapWidth = atoi(tokens[2].c_str());
	float mapHeight = atoi(tokens[3].c_str());
	map = new CTileMap(idTex, tokens[0]);
	CCamera::GetInstance()->SetMapSize(mapWidth, mapHeight);
}

void CPlayScene::Load()
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

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (i == 2) continue;
		if (!objects[i]->isDie || !objects[i]->isDead)
			coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		if (!objects[i]->isDead)
			objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario */// FORGET: NHỚ BỎ CHỖ NÀY SAU VÌ ĐÃ CÓ CAMERA MỚI
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;

	CGame::GetInstance()->SetCamPos(cx, cy);

	CCamera::GetInstance()->Update(player);
}

void CPlayScene::Render()
{
	if (map != NULL)
	{
		map->Render(player);
	}

	for (int i = 0; i < objects.size(); i++)
	{
		if (!objects[i]->isDead)
			objects[i]->Render();
	}
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScene::UpdateCamera(int mapWidth, int mapHeight)
{
	//tính fps của game
	/*DWORD endRender = GetTickCount();
	if (endRender - beginRender > 0 && endRender - beginRender < 1000)
		ifps = 1000 / (endRender - beginRender);
	else
		ifps = 1000;
	beginRender = endRender;*/

	// test CCamera move when Mario is not on center screen
	//mCamera->SetPosition(mPlayer->GetPosition() + D3DXVECTOR3(100,0,0));
	float x, y;
	player->GetPosition(x, y);
	CCamera::GetInstance()->SetPosition(x, y);

	if (CCamera::GetInstance()->GetBound().left < 0)
	{
		//vi position cua CCamera::GetInstance() ma chinh giua CCamera::GetInstance()
		//luc nay o vi tri goc ben trai cua the gioi thuc
		CCamera::GetInstance()->SetPosition(CCamera::GetInstance()->GetWidth() / 2, CCamera::GetInstance()->GetPosition().y);
	}

	if (CCamera::GetInstance()->GetBound().right > mapWidth)
	{
		//luc nay cham goc ben phai cua the gioi thuc
		CCamera::GetInstance()->SetPosition(mapWidth - CCamera::GetInstance()->GetWidth() / 2,
			CCamera::GetInstance()->GetPosition().y);
	}

	if (CCamera::GetInstance()->GetBound().top < 0)
	{
		//luc nay cham goc tren the gioi thuc
		CCamera::GetInstance()->SetPosition(CCamera::GetInstance()->GetPosition().x, CCamera::GetInstance()->GetHeight() / 2);
	}

	if (CCamera::GetInstance()->GetBound().bottom > mapHeight)
	{
		//luc nay cham day cua the gioi thuc
		CCamera::GetInstance()->SetPosition(CCamera::GetInstance()->GetPosition().x,
			mapHeight - CCamera::GetInstance()->GetHeight() / 2);
	}
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	/*DebugOut(L"[KEYDOWN] KeyDown: %d\n", KeyCode);*/

	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_S:
		if (mario->canJump)
			mario->vy = -0.15f;
		mario->canRepeatJump = false;
		//mario->canJump = false;
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON)
		{	
			if (mario->state == MARIO_STATE_RUNJUMP)
			{
				if (mario->isOnGround)
				{
					mario->fly_limit_start = GetTickCount();
				}
				mario->fly_start = GetTickCount();
				mario->SetState(MARIO_STATE_FLY);
			}
			else if (!mario->isOnGround && mario->vy >= 0)
			{
				mario->wag_start = GetTickCount();
				mario->SetState(MARIO_STATE_WAG);
			}
		}
		break;
	case DIK_A:
		mario->canHold = true;

		if (mario->GetLevel() == MARIO_LEVEL_RACCOON)
		{
			if (mario->canAttack)
			{
				mario->tail_start = GetTickCount();
				mario->SetState(MARIO_STATE_TAIL);
				mario->canAttack = false;
			}
		}
		if (mario->GetLevel() == MARIO_LEVEL_FIRE)
		{
			if (mario->canAttack)
			{
				int bulletCount = 0;				
				for (int i = 0; i < mario->bullets->size(); i ++)
				{
					CBullet* bullet = mario->bullets->at(i);

					if (!bullet->isDead)
						continue;

					float bx, by;
					mario->GetPosition(bx, by);
					bullet->SetPosition(bx, by);
					bullet->vx = mario->nx * BULLET_VX_SPEED;
					bullet->vy = BULLET_VY_SPEED;
					bullet->SetState(BULLET_STATE_FIRE);
					bulletCount++;
					break;
				}

				if (bulletCount != 0)
				{
					mario->fire_start = GetTickCount();					
					mario->canAttack = false;
					if (mario->isOnGround)
						mario->SetState(MARIO_STATE_SHOT);
					else if (mario->state == MARIO_STATE_JUMP_HIGH || mario->state == MARIO_STATE_JUMP_SHORT)
						mario->SetState(MARIO_STATE_JUMP_SHOT);
					else if (mario->state == MARIO_STATE_RUNJUMP)
						mario->SetState(MARIO_STATE_RUNJUMP_SHOT);
				}
			}
		}
		break;
	case DIK_X:
		mario->SetState(MARIO_STATE_JUMP_SHORT);
		break;
		/*case DIK_DOWN:
			mario->canDuck = true;
			break;*/
	case DIK_R:
		mario->Reset();
		break;
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		if (mario->GetLevel() == MARIO_LEVEL_SMALL)
			mario->y -= MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT;
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		if (mario->GetLevel() == MARIO_LEVEL_SMALL)
			mario->y -= MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT;
		mario->SetLevel(MARIO_LEVEL_RACCOON);
		break;
	case DIK_4:
		if (mario->GetLevel() == MARIO_LEVEL_SMALL)
			mario->y -= MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT;
		mario->SetLevel(MARIO_LEVEL_FIRE);
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	/*DebugOut(L"[KEYUP] KeyUp: %d\n", KeyCode);*/

	CGame* game = CGame::GetInstance();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();

	switch (KeyCode)
	{
	case DIK_X:
		break;
	case DIK_S:
		mario->canJump = false;
		mario->canRepeatJump = true;
		break;
	case DIK_A:		
		mario->canHold = false;	

		if (mario->isHold)
		{			
			if (mario->koopas != NULL)	
			{
				mario->kick_start = GetTickCount();
				mario->koopas->SetState(KOOPAS_STATE_SPIN);
				mario->koopas->vx = mario->nx * KOOPAS_SPIN_SPEED;
				mario->koopas = NULL;
				mario->isHold = false;
			}
		}
		else if (mario->state == MARIO_STATE_PREPARE_RUN || mario->state == MARIO_STATE_RUN)
		{
			mario->SetState(MARIO_STATE_WALKING);
			mario->vx = mario->nx * MARIO_WALKING_SPEED;
		}
		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	//DebugOut(L"[KEYSTATE] KeyState: %d\n", states);

	CGame* game = CGame::GetInstance();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	int state = mario->GetState();
	// disable control key when Mario die 
	if (state == MARIO_STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT))
	{
		mario->nx = 1;
		if (state != MARIO_STATE_DUCK)
			if (mario->vx < MARIO_WALKING_SPEED)
				mario->vx += 0.005f;

		// Phanh
		if (mario->vx < 0 && mario->vx < -0.07f && mario->isOnGround)
			mario->SetState(MARIO_STATE_SKID);
		else if (mario->vx >= 0 && mario->vy == 0 && state != MARIO_STATE_PREPARE_RUN && state != MARIO_STATE_RUN)
			mario->SetState(MARIO_STATE_WALKING);
		if (game->IsKeyDown(DIK_A))
		{
			if (state != MARIO_STATE_RUN)
			{
				if (mario->vx < mario->nx * MARIO_PREPARE_RUN_SPEED)
					mario->vx += 0.0015f;
			}

			if (state == MARIO_STATE_WALKING)
			{
				if (mario->vx >= mario->nx * MARIO_PREPARE_RUN_SPEED)
				{
					mario->run_start = GetTickCount();
					mario->SetState(MARIO_STATE_PREPARE_RUN);
				}
			}
			else if (state == MARIO_STATE_PREPARE_RUN && GetTickCount() - mario->run_start >= MARIO_RUN_TIME / 3)
			{
				mario->SetState(MARIO_STATE_RUN);
			}
		}
		/*else if (mario->vy == 0)
		{
			mario->SetState(MARIO_STATE_WALKING);
			mario->vx = MARIO_WALKING_SPEED;
		}*/
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		mario->nx = -1;
		if (state != MARIO_STATE_DUCK)
			if (mario->vx > -MARIO_WALKING_SPEED)
				mario->vx -= 0.005f;
			
		// Phanh
		if (mario->vx > 0 && mario->vx > 0.07f && mario->isOnGround)
			mario->SetState(MARIO_STATE_SKID);
		else if (mario->vx <= 0 && mario->vy == 0 && state != MARIO_STATE_PREPARE_RUN && state != MARIO_STATE_RUN)
			mario->SetState(MARIO_STATE_WALKING);

		if (game->IsKeyDown(DIK_A))
		{
			/*if (state != MARIO_STATE_RUN)
				mario->vx = 1.5 * mario->nx * MARIO_WALKING_SPEED;*/

			if (state != MARIO_STATE_RUN)
			{
				if (mario->vx > mario->nx * MARIO_PREPARE_RUN_SPEED)
					mario->vx -= 0.0015f;
			}

			if (state == MARIO_STATE_WALKING)
			{
				if (mario->vx <= mario->nx * MARIO_PREPARE_RUN_SPEED)
				{
					mario->run_start = GetTickCount();
					mario->SetState(MARIO_STATE_PREPARE_RUN);
				}
			}
			else if (state == MARIO_STATE_PREPARE_RUN && GetTickCount() - mario->run_start >= MARIO_RUN_TIME / 3)
			{
				mario->SetState(MARIO_STATE_RUN);
			}
		}
		/*else if (mario->vy == 0)
			mario->SetState(MARIO_STATE_WALKING);*/
	}
	/*else if (!mario->isOnGround)
	{
		mario->SetState(MARIO_STATE_JUMP_HIGH);
	}*/
	else if (mario->isOnGround)
	{
		if (mario->nx > 0)
		{
			mario->SetState(MARIO_STATE_WALKING);
			mario->vx -= 0.004f;
			if (mario->vx <= 0)
				mario->SetState(MARIO_STATE_IDLE);
		}
		if (mario->nx < 0)
		{
			mario->SetState(MARIO_STATE_WALKING);
			mario->vx += 0.004f;
			if (mario->vx >= 0)
				mario->SetState(MARIO_STATE_IDLE);
		}
		//mario->SetState(MARIO_STATE_IDLE);
	}	
	if (game->IsKeyDown(DIK_S) && mario->canJump)
	{
		if (mario->canJumpHigher)
			mario->vy -= MARIO_JUMP_HIGH_SPEED_Y;
		if (abs(mario->vx) == MARIO_RUN_SPEED)
			mario->SetState(MARIO_STATE_RUNJUMP);
		else
			mario->SetState(MARIO_STATE_JUMP_HIGH);
	}
	if (game->IsKeyDown(DIK_DOWN) && mario->isOnGround && mario->GetLevel() != MARIO_LEVEL_SMALL)
	{
		mario->SetState(MARIO_STATE_DUCK);
	}
	if (game->IsKeyDown(DIK_A) && mario->isHold)
	{
		/*mario->SetState(MARIO_STATE_IDLE_HOLD);*/
		if (mario->koopas != NULL)
		{
			float l, t, r, b;
			mario->GetBoundingBox(l, t, r, b);

			if (mario->nx > 0)
			{
				if (mario->GetLevel() == MARIO_LEVEL_SMALL)
					mario->koopas->SetPosition(r - 4, t - MARIO_SMALL_BBOX_HEIGHT / 4);
				else
					mario->koopas->SetPosition(r - 4, t + MARIO_BIG_BBOX_HEIGHT / 9);
			}
			else
			{
				if (mario->GetLevel() == MARIO_LEVEL_SMALL)
					mario->koopas->SetPosition(l - (mario->koopas->right - mario->koopas->left) + 4, t - MARIO_SMALL_BBOX_HEIGHT / 4);
				else
					mario->koopas->SetPosition(l - (mario->koopas->right - mario->koopas->left) + 4, t + MARIO_BIG_BBOX_HEIGHT / 9);
			}
		}
	}
	/*if (game->IsKeyDown(DIK_A))
	{
		if (state != MARIO_STATE_IDLE && mario->vx != 0)
		{
			mario->vx = 1.5 * mario->nx * MARIO_WALKING_SPEED;

			if (state == MARIO_STATE_WALKING)
			{
				mario->run_start = GetTickCount();
				mario->SetState(MARIO_STATE_PREPARE_RUN);
			}
			else if (state == MARIO_STATE_PREPARE_RUN && GetTickCount() - mario->run_start >= MARIO_RUN_TIME / 3)
			{
				mario->SetState(MARIO_STATE_RUN);
			}
		}
	}*/
}