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

#define SCENE_SECTION_UNKNOWN			-1
#define SCENE_SECTION_TEXTURES			2
#define SCENE_SECTION_SPRITES			3
#define SCENE_SECTION_ANIMATIONS		4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS			6
#define SCENE_SECTION_MAP				7
#define SCENE_SECTION_GRID				8

#define OBJECT_TYPE_MARIO				0
#define OBJECT_TYPE_BRICK				1
#define OBJECT_TYPE_GOOMBA				2
#define OBJECT_TYPE_KOOPAS				3
#define OBJECT_TYPE_BOX					4
#define OBJECT_TYPE_GROUND				5
#define OBJECT_TYPE_QUESTION_BRICK		6
#define OBJECT_TYPE_WARPPIPE			7
#define OBJECT_TYPE_BREAKABLE_BRICK		9
#define OBJECT_TYPE_MUSHROOM			10
#define OBJECT_TYPE_LEAF				11
#define OBJECT_TYPE_COIN				12
#define OBJECT_TYPE_CLOUD_TOOTH			13
#define OBJECT_TYPE_PARA_GOOMBA			14

#define OBJECT_TYPE_PORTAL				50
#define OBJECT_TYPE_HUD					51

//#define MAX_SCENE_LINE					1024


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

	if (ani_set_id != -1)
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
	int objId = atoi(tokens[1].c_str());
	float x = atof(tokens[2].c_str());
	float y = atof(tokens[3].c_str());

	int ani_set_id = atoi(tokens[4].c_str());

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
		obj = new CMario(x, y);
		player = (CMario*)obj;

		//for (int i = 0; i < 2; i++)
		//{
		//	CBullet* bullet = new CBullet();
		//	//bullet->SetAnimationSet(ani_set);
		//	player->bullets->push_back(bullet);
		//	objects.push_back(player->bullets->at(i));
		//}

		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_QUESTION_BRICK:
	{
		int type = atoi(tokens[5].c_str());
		obj = new CQuestionBrick(x, y, type);
		break;
	}
	case OBJECT_TYPE_BREAKABLE_BRICK:
	{
		int type = atoi(tokens[5].c_str());
		obj = new CBreakableBrick(type);
		break;
	}
	case OBJECT_TYPE_MUSHROOM: obj = new CMushRoom(); break;
	case OBJECT_TYPE_LEAF: obj = new CLeaf(); break;
	case OBJECT_TYPE_KOOPAS:
	{
		int type = atoi(tokens[5].c_str());
		/*if (type == 2)
		{
			float min = atof(tokens[5].c_str());
			float max = atof(tokens[6].c_str());
			obj = new CKoopas(type, min, max);
		}
		else*/
		obj = new CKoopas(type);
	}
	break;
	case OBJECT_TYPE_BOX:
	{
		float r = atof(tokens[5].c_str());
		float b = atof(tokens[6].c_str());
		obj = new CBox(r, b);
	}
	break;
	case OBJECT_TYPE_WARPPIPE:
	{
		float r = atof(tokens[5].c_str());
		float b = atof(tokens[6].c_str());

		int type = -1;
		if (tokens.size() > 7)
			type = atoi(tokens[7].c_str());

		float tele_x = -1, tele_y = -1;
		if (tokens.size() > 9)
		{
			tele_x = atof(tokens[8].c_str());
			tele_y = atof(tokens[9].c_str());
		}
		obj = new CWarpPipe(x, y, r, b, type, tele_x, tele_y);
	}
	break;
	case OBJECT_TYPE_GROUND:
	{
		float r = atof(tokens[5].c_str());
		float b = atof(tokens[6].c_str());
		obj = new CGround(r, b);
	}
	break;
	case OBJECT_TYPE_PORTAL:
	{		
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y,scene_id);
	}
	break;
	case OBJECT_TYPE_COIN:	obj = new CCoin(); break;
	case OBJECT_TYPE_CLOUD_TOOTH:	obj = new CCloudTooth(); break;
	case OBJECT_TYPE_PARA_GOOMBA:	obj = new CParaGoomba(); break;
	case OBJECT_TYPE_HUD:
	{
		hud = new CHud();
		break;
	}
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	if (obj != NULL)
	{
		obj->SetId(objId);
		obj->SetPosition(x, y);
		obj->SetRespawnPosition(x, y);

		/*LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);*/

		if (ani_set_id != -1)
			obj->SetAnimationSet(ani_set);

		if (!dynamic_cast<CMario*>(obj))
			objects.push_back(obj);
	}
}

void CPlayScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;

	int idTexMap = atoi(tokens[1].c_str());
	float mapWidth = atoi(tokens[2].c_str());
	float mapHeight = atoi(tokens[3].c_str());
	map = new CTileMap(idTexMap, tokens[0]);

	switch (map->GetId())
	{
	case MAP_1_1:
		CCamera::GetInstance()->SetMapSize(LEFT_MAP_1_1, TOP_MAP_1_1, RIGHT_MAP_1_1, BOTTOM_MAP_1_1, WIDTH_MAP_1_1, HEIGHT_MAP_1_1);
	default:
		break;
	}
}

void CPlayScene::_ParseSection_GRID(string line)
{
	vector<string> tokens = split(line);
	
	if (tokens.size() < 1) return;

	switch (map->GetId())
	{
	case MAP_1_1:
		grid = new CGrid(WIDTH_MAP_1_1, HEIGHT_ALL_MAP_1_1, CELL_SIZE);
	default:
		break;
	}

	ifstream f;
	f.open(tokens[0]);

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		vector<string> tokens = split(line, " ");

		if (tokens.size() < 1) return;

		int cellIndex = 0;
		int objIndex = 0;

		cellIndex = atoi(tokens[0].c_str());
		for (int i = 1; i < tokens.size(); i++)
		{
			objIndex = atoi(tokens[i].c_str());
			for (int t = 0; t < objects.size(); t++)
			{
				if (objects[t]->id == objIndex)
				{
					// Add obj into cell of grid
					grid->AddObjToCell(cellIndex, objects[t]);
					objects[t]->listCellIndex.push_back(cellIndex);
					// Case WarpPipe has Piranha
					if (dynamic_cast<CWarpPipe*>(objects[t]))
					{
						CWarpPipe* warpPipe = dynamic_cast<CWarpPipe*>(objects[t]);
						if (warpPipe->GetPiranha() != NULL)
							grid->AddObjToCell(cellIndex, warpPipe->GetPiranha());
					}
					break;
				}
			}
		}
	}

	f.close();
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
		if (line == "[GRID]") {
			section = SCENE_SECTION_GRID; continue;
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
		case SCENE_SECTION_GRID: _ParseSection_GRID(line); break;
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

	// calculate obj in view
	viewOtherObjs.clear();
	viewObjs.clear();
	viewAfterObjs.clear();
	/*CGame::GetInstance()->CalcViewObjs(&viewOtherObjs, otherObjs);
	CGame::GetInstance()->CalcViewObjs(&viewObjs, objects);*/	
	grid->CalcColliableObjs(CCamera::GetInstance(), viewObjs, viewAfterObjs);
	DebugOut(L"[Obj]: %d\n", viewObjs.size());
	DebugOut(L"[AfterObj]: %d\n", viewAfterObjs.size());
	DebugOut(L"[BehindObj]: %d\n", behindObjs.size());
	DebugOut(L"[FrontObj]: %d\n", frontObjs.size());

	// Cal colliable objs
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < behindObjs.size(); i++)
	{
		if (behindObjs[i]->isActive && !behindObjs[i]->isDie && !behindObjs[i]->isDead)
			coObjects.push_back(behindObjs[i]);
	}
	for (size_t i = 0; i < viewObjs.size(); i++)
	{
		/*if (i == 2) continue;*/
		if (dynamic_cast<CMario*>(viewObjs.at(i)))
			continue;
		if (viewObjs[i]->isActive && !viewObjs[i]->isDie && !viewObjs[i]->isDead)
			coObjects.push_back(viewObjs[i]);
	}
	for (size_t i = 0; i < viewAfterObjs.size(); i++)
	{
		if (viewAfterObjs[i]->isActive && !viewAfterObjs[i]->isDie && !viewAfterObjs[i]->isDead)
			coObjects.push_back(viewAfterObjs[i]);
	}
	for (size_t i = 0; i < frontObjs.size(); i++)
	{
		if (frontObjs[i]->isActive && !frontObjs[i]->isDie && !frontObjs[i]->isDead)
			coObjects.push_back(frontObjs[i]);
	}

	// Update all objs: otherObjs -> objs -> Mario -> afterObjs
	for (size_t i = 0; i < behindObjs.size(); i++)
	{
		if (!behindObjs[i]->isDead)
			behindObjs[i]->Update(dt, &coObjects);
	}
	for (size_t i = 0; i < viewObjs.size(); i++)
	{
		if (!viewObjs[i]->isDead)
			viewObjs[i]->Update(dt, &coObjects);
	}
	if (player != NULL)
		player->Update(dt, &coObjects);
	for (size_t i = 0; i < viewAfterObjs.size(); i++)
	{
		if (!viewAfterObjs[i]->isDead)
			viewAfterObjs[i]->Update(dt, &coObjects);
	}
	for (size_t i = 0; i < frontObjs.size(); i++)
	{
		if (!frontObjs[i]->isDead)
			frontObjs[i]->Update(dt, &coObjects);
	}
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	hud->Update(dt);	

	// Update camera to follow mario */// FORGET: NHỚ BỎ CHỖ NÀY SAU VÌ ĐÃ CÓ CAMERA MỚI
	/*float cx, cy;
	player->GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;

	CGame::GetInstance()->SetCamPos(cx, cy);*/

	CCamera::GetInstance()->Update(player);
}

void CPlayScene::Render()
{
	if (map != NULL)
	{
		map->Render();
	}

	viewOtherObjs.clear();
	viewObjs.clear();
	viewAfterObjs.clear();
	/*CGame::GetInstance()->CalcViewObjs(&viewOtherObjs, otherObjs);
	CGame::GetInstance()->CalcViewObjs(&viewObjs, objects);*/
	grid->CalcColliableObjs(CCamera::GetInstance(), viewObjs, viewAfterObjs);

	// Render all objs: otherObjs -> objs -> Mario -> afterObjs
	for (int i = 0; i < behindObjs.size(); i++)
	{
		if (behindObjs[i]->isActive && !behindObjs[i]->isDead)
			behindObjs[i]->Render();
	}
	for (int i = 0; i < viewObjs.size(); i++)
	{
		if (viewObjs[i]->isActive && !viewObjs[i]->isDead)
		{
			if (dynamic_cast<CMario*>(viewObjs[i]))
				continue;
			viewObjs[i]->Render();
		}
	}
	if (player != NULL)
		player->Render();
	for (int i = 0; i < viewAfterObjs.size(); i++)
	{
		if (viewAfterObjs[i]->isActive && !viewAfterObjs[i]->isDead)
			viewAfterObjs[i]->Render();
	}
	for (int i = 0; i < frontObjs.size(); i++)
	{
		if (frontObjs[i]->isActive && !frontObjs[i]->isDead)
			frontObjs[i]->Render();
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;	

	hud->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	// view objs
	/*for (int i = 0; i < viewOtherObjs.size(); i++)
		delete viewOtherObjs[i];*/
	viewOtherObjs.clear();

	/*for (int i = 0; i < viewObjs.size(); i++)
		delete viewObjs[i];*/
	viewObjs.clear();
	viewAfterObjs.clear();

	// behind objs
	for (int i = 0; i < behindObjs.size(); i++)
		delete behindObjs[i];
	behindObjs.clear();

	// front objs
	for (int i = 0; i < frontObjs.size(); i++)
		delete frontObjs[i];
	frontObjs.clear();

	// objs
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];
	objects.clear();	

	delete player;
	player = NULL;

	delete grid;
	delete map;
	delete hud;
	grid = NULL;
	map = NULL;
	hud = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScene::EndScene()
{
	hud->EndScene();
}

//void CPlayScene::UpdateCamera(int mapWidth, int mapHeight)
//{
//	//tính fps của game
//	/*DWORD endRender = GetTickCount();
//	if (endRender - beginRender > 0 && endRender - beginRender < 1000)
//		ifps = 1000 / (endRender - beginRender);
//	else
//		ifps = 1000;
//	beginRender = endRender;*/
//
//	// test CCamera move when Mario is not on center screen
//	//mCamera->SetPosition(mPlayer->GetPosition() + D3DXVECTOR3(100,0,0));
//	float x, y;
//	player->GetPosition(x, y);
//	CCamera::GetInstance()->SetPosition(x, y);
//
//	if (CCamera::GetInstance()->GetBound().left < 0)
//	{
//		//vi position cua CCamera::GetInstance() ma chinh giua CCamera::GetInstance()
//		//luc nay o vi tri goc ben trai cua the gioi thuc
//		CCamera::GetInstance()->SetPosition(CCamera::GetInstance()->GetWidth() / 2, CCamera::GetInstance()->GetPosition().y);
//	}
//
//	if (CCamera::GetInstance()->GetBound().right > mapWidth)
//	{
//		//luc nay cham goc ben phai cua the gioi thuc
//		CCamera::GetInstance()->SetPosition(mapWidth - CCamera::GetInstance()->GetWidth() / 2,
//			CCamera::GetInstance()->GetPosition().y);
//	}
//
//	if (CCamera::GetInstance()->GetBound().top < 0)
//	{
//		//luc nay cham goc tren the gioi thuc
//		CCamera::GetInstance()->SetPosition(CCamera::GetInstance()->GetPosition().x, CCamera::GetInstance()->GetHeight() / 2);
//	}
//
//	if (CCamera::GetInstance()->GetBound().bottom > mapHeight)
//	{
//		//luc nay cham day cua the gioi thuc
//		CCamera::GetInstance()->SetPosition(CCamera::GetInstance()->GetPosition().x,
//			mapHeight - CCamera::GetInstance()->GetHeight() / 2);
//	}
//}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	/*DebugOut(L"[KEYDOWN] KeyDown: %d\n", KeyCode);*/

	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->state == MARIO_STATE_DIE || mario->state == MARIO_STATE_END_SCENE) return;
	
	switch (KeyCode)
	{
	case DIK_C:
		CGame::GetInstance()->SwitchScene(WORLD_MAP_1);
		break;
	case DIK_V:
		CGame::GetInstance()->SwitchScene(TITLE_SCREEN);
		break;
	case DIK_NUMPAD9:
		mario->SetPosition(1400, 130);
		CCamera::GetInstance()->SetIsStatic(false);
		break;
	case DIK_NUMPAD1:
		mario->SetPosition(250, 350);
		CCamera::GetInstance()->SetIsStatic(true);
		break;
	case DIK_NUMPAD2:
		mario->SetPosition(500, 350);
		CCamera::GetInstance()->SetIsStatic(true);
		break;
	case DIK_NUMPAD3:
		mario->SetPosition(680, 350);
		CCamera::GetInstance()->SetIsStatic(true);
		break;
	case DIK_NUMPAD4:
		mario->SetPosition(1300, 350);
		CCamera::GetInstance()->SetIsStatic(true);
		break;
	case DIK_NUMPAD5:
		mario->SetPosition(1950, 350);
		CCamera::GetInstance()->SetIsStatic(true);
		break;
	case DIK_NUMPAD6:
		mario->SetPosition(2260, 50);
		CCamera::GetInstance()->SetIsStatic(false);
		break;
	case DIK_NUMPAD7:
		mario->SetPosition(2600, 350);
		CCamera::GetInstance()->SetIsStatic(false);
		break;
	case DIK_NUMPAD0:
		mario->SetPosition(2120, 500);
		CCamera::GetInstance()->SetIsStatic(false);
		break;
	case DIK_S:
		if (mario->canJump)
			mario->vy = -0.15f;
		mario->canRepeatJump = false;
		//mario->canJump = false;
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON)
		{
			if (/*mario->state == MARIO_STATE_RUNJUMP*/mario->GetPower() == 7)
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
				for (int i = 0; i < mario->bullets->size(); i++)
				{
					CBullet* bullet = mario->bullets->at(i);

					if (!bullet->isDead)
						continue;

					float bx, by;
					mario->GetPosition(bx, by);
					bullet->SetPosition(bx, by);
					bullet->vx = mario->nx * BULLET_X_SPEED;
					bullet->vy = BULLET_Y_SPEED;
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
	if (mario->state == MARIO_STATE_DIE || mario->state == MARIO_STATE_END_SCENE) return;

	switch (KeyCode)
	{
	case DIK_DOWN:
		if (!mario->canAttack)
			mario->canAttack = true;
		break;
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
				mario->SetState(MARIO_STATE_KICK);
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
	if (state == MARIO_STATE_DIE || state == MARIO_STATE_END_SCENE) return;
	if (game->IsKeyDown(DIK_RIGHT) && state != MARIO_STATE_PIPE)
	{
		mario->nx = 1;
		if (state != MARIO_STATE_DUCK /*&& !mario->isPreventMoveX*/)
			if (mario->vx < MARIO_WALKING_SPEED)
				mario->vx += 0.005f;

		if (state == MARIO_STATE_WALKING && mario->colX != 0)
		{
			mario->DecreasePower();
		}
		// Phanh
		if (mario->vx < 0 && mario->vx < -0.07f && mario->isOnGround && !mario->isHold)
		{
			mario->SetState(MARIO_STATE_SKID);
			mario->DecreasePower();
		}
		else if (mario->vx >= 0 && mario->vy == 0 && state != MARIO_STATE_PREPARE_RUN && state != MARIO_STATE_RUN && state != MARIO_STATE_TAIL)
			mario->SetState(MARIO_STATE_WALKING);
		if (game->IsKeyDown(DIK_A))
		{
			if (mario->isOnGround && state != MARIO_STATE_SKID)
				mario->IncreasePower();
			if (/*state != MARIO_STATE_RUN*/ mario->GetPower() < 7)
			{
				if (mario->vx < mario->nx * MARIO_PREPARE_RUN_SPEED)
					mario->vx += 0.0015f;
			}

			if (state == MARIO_STATE_WALKING)
			{
				if (mario->vx >= mario->nx * MARIO_PREPARE_RUN_SPEED)
				{
					mario->SetState(MARIO_STATE_PREPARE_RUN);
				}
			}
			else if (/*state == MARIO_STATE_PREPARE_RUN && !mario->isHold && GetTickCount() - mario->run_start >= MARIO_RUN_TIME / 6*/state == MARIO_STATE_PREPARE_RUN && !mario->isHold && mario->GetPower() == 7)
			{
				mario->SetState(MARIO_STATE_RUN);
			}
		}
		else if (mario->isOnGround)
		{
			mario->DecreasePower();
		}
		/*else if (mario->vy == 0)
		{
			mario->SetState(MARIO_STATE_WALKING);
			mario->vx = MARIO_WALKING_SPEED;
		}*/
	}
	else if (game->IsKeyDown(DIK_LEFT) && state != MARIO_STATE_PIPE)
	{
		mario->nx = -1;
		if (state != MARIO_STATE_DUCK /*&& !mario->isPreventMoveX*/)
			if (mario->vx > -MARIO_WALKING_SPEED)
				mario->vx -= 0.005f;

		if (state == MARIO_STATE_WALKING && mario->colX != 0)
		{
			mario->DecreasePower();
		}
		// Phanh
		if (mario->vx > 0 && mario->vx > 0.07f && mario->isOnGround && !mario->isHold)
		{
			mario->SetState(MARIO_STATE_SKID);
			mario->DecreasePower();
		}
		else if (mario->vx <= 0 && mario->vy == 0 && state != MARIO_STATE_PREPARE_RUN && state != MARIO_STATE_RUN && state != MARIO_STATE_TAIL)
			mario->SetState(MARIO_STATE_WALKING);

		if (game->IsKeyDown(DIK_A))
		{
			/*if (state != MARIO_STATE_RUN)
				mario->vx = 1.5 * mario->nx * MARIO_WALKING_SPEED;*/
			if (mario->isOnGround && state != MARIO_STATE_SKID)
				mario->IncreasePower();
			if (/*state != MARIO_STATE_RUN*/ mario->GetPower() < 7)
			{
				if (mario->vx > mario->nx * MARIO_PREPARE_RUN_SPEED)
					mario->vx -= 0.0015f;
			}

			if (state == MARIO_STATE_WALKING)
			{
				if (mario->vx <= mario->nx * MARIO_PREPARE_RUN_SPEED)
				{
					mario->SetState(MARIO_STATE_PREPARE_RUN);
				}
			}
			else if (/*state == MARIO_STATE_PREPARE_RUN && !mario->isHold && GetTickCount() - mario->run_start >= MARIO_RUN_TIME / 3*/state == MARIO_STATE_PREPARE_RUN && !mario->isHold && mario->GetPower() == 7)
			{
				mario->SetState(MARIO_STATE_RUN);
			}
		}
		else if (mario->isOnGround)
		{
			mario->DecreasePower();
		}
		/*else if (mario->vy == 0)
			mario->SetState(MARIO_STATE_WALKING);*/
	}
	/*else if (!mario->isOnGround)
	{
		mario->SetState(MARIO_STATE_JUMP_HIGH);
	}*/
	else if (mario->isOnGround && mario->kick_start == 0 && mario->tail_start == 0)
	{
		if (mario->GetPower() > 0)
		{
			mario->DecreasePower();
		}
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
	if (game->IsKeyDown(DIK_DOWN) && mario->isOnGround && mario->GetLevel() != MARIO_LEVEL_SMALL && !mario->isHold)
	{
		mario->SetState(MARIO_STATE_DUCK);
	}
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