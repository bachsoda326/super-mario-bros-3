#include "TitleScene.h"
#include <iostream>
#include <fstream>

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6

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
#define OBJECT_TYPE_FLY_BAR				15
#define OBJECT_TYPE_TITLE_BACKGROUND	16
#define OBJECT_TYPE_LUGI				17

#define MAX_SCENE_LINE 1024

CTitleScene::CTitleScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CTitleSceneKeyHandler(this);
	CCamera::GetInstance()->SetPosition(CGame::GetInstance()->GetScreenWidth() / 2, CGame::GetInstance()->GetScreenHeight() / 2);
	CCamera::GetInstance()->SetMapSize(LEFT_TITLE_SCENE - 24, TOP_TITLE_SCENE - 50, RIGHT_TITLE_SCENE + 24, BOTTOM_TITLE_SCENE, WIDTH_TITLE_SCENE, HEIGHT_TITLE_SCENE);
}

void CTitleScene::_ParseSection_TEXTURES(string line)
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

void CTitleScene::_ParseSection_SPRITES(string line)
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

void CTitleScene::_ParseSection_ANIMATIONS(string line)
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

void CTitleScene::_ParseSection_ANIMATION_SETS(string line)
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

void CTitleScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

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
		obj = new CMario(x, y);
		mario = (CMario*)obj;
		mario->SetLevel(MARIO_LEVEL_BIG);
		mario->nx = -1;
		mario->Dead();
		break;
	case OBJECT_TYPE_LUGI:
		obj = new CMario(x, y);
		lugi = (CMario*)obj;
		lugi->SetLevel(MARIO_LEVEL_BIG);
		lugi->Dead();
		break;
	case OBJECT_TYPE_GOOMBA:
	{
		obj = new CGoomba();
		obj->vx = 0;
		obj->Dead();
		fallingObjs.push_back(obj);
		break;
	}
	case OBJECT_TYPE_MUSHROOM:
	{
		obj = new CMushRoom();
		((CMushRoom*)obj)->isInitialized = true;
		obj->Dead();
		fallingObjs.push_back(obj);
		break;
	}
	case OBJECT_TYPE_LEAF:
	{
		obj = new CLeaf();
		((CLeaf*)obj)->isInitialized = true;
		((CLeaf*)obj)->start_x = x;
		obj->Dead();
		fallingObjs.push_back(obj);
		break;
	}
	case OBJECT_TYPE_KOOPAS:
	{
		int type = atoi(tokens[5].c_str());
		obj = new CKoopas(type);
		obj->Dead();
		if (type == KOOPAS_HIDE)
			fallingObjs.push_back(obj);
		else
			listKoopas.push_back(obj);
	}
	break;
	case OBJECT_TYPE_GROUND:
	{
		float r = atof(tokens[5].c_str());
		float b = atof(tokens[6].c_str());
		obj = new CGround(r, b);
	}
	break;
	case OBJECT_TYPE_TITLE_BACKGROUND:
		obj = new CTitleBackGround();
		backGround = (CTitleBackGround*)obj;
		break;
	}

	// General object setup
	if (obj != NULL)
	{
		obj->isOnTitleScene = true;
		obj->SetId(objId);
		obj->SetPosition(x, y);
		obj->SetRespawnPosition(x, y);

		if (ani_set_id != -1)
			obj->SetAnimationSet(ani_set);
		objects.push_back(obj);
	}
}

void CTitleScene::Load()
{
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
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
}

void CTitleScene::Update(DWORD dt)
{
	HandleMario();
	HandleLugi();

	// Begin scene 1: Active Mario & Lugi
	if (backGround->isScene1Begin)
	{
		mario->Alive();
		lugi->Alive();
		mario->SetState(MARIO_STATE_WALKING);
		lugi->SetState(MARIO_STATE_WALKING);
		backGround->isScene1Begin = false;
	}

	// Begin scene 2:
	if (backGround->isScene2Begin)
	{
		mario->vx = -0.07f;
		lugi->vx = 0.07f;
		if (lugi->x >= 90)
		{
			lugi->SetState(MARIO_STATE_JUMP_HIGH);
			lugi->vy = -0.22f;
			backGround->isScene2Begin = false;
		}
		/*else if (lugi->state == MARIO_STATE_JUMP_HIGH)
		{
			backGround->isScene2Begin = false;
		}*/
	}
	if (lugi->y < 0 && lugi->state == MARIO_STATE_JUMP_HIGH)
	{
		lugi->vx = 0.07f;
		backGround->isScene3Begin = true;
		backGround->isScene2Begin = false;
	}

	// End scene 3
	if (backGround->isScene3End)
	{
		for (size_t i = 0; i < fallingObjs.size(); i++)
		{
			if (fallingObjs[i]->isDead)
				fallingObjs[i]->Alive();
		}
		fallingObjs.clear();
	}

	/*backGround->Update(dt);*/
	// Cal colliable objs
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<CTitleBackGround*>(objects[i]))
			continue;
		if (!objects[i]->isDie)
			coObjects.push_back(objects[i]);
	}

	// Update objs
	for (size_t i = 2; i < objects.size(); i++)
	{
		if (!objects[i]->isDie)
			objects[i]->Update(dt, &coObjects);
	}
	if (!mario->isDie)
		mario->Update(dt, &coObjects);
	if (!lugi->isDie)
		lugi->Update(dt, &coObjects);
}

void CTitleScene::Render()
{
	/*backGround->Render();*/
	for (int i = 2; i < objects.size(); i++)
	{
		if (!objects[i]->isDead)
			objects[i]->Render();
	}
	if (!mario->isDead)
		mario->Render();
	if (!lugi->isDead)
		lugi->Render();
}

void CTitleScene::Unload()
{
	mario = NULL;
	lugi = NULL;
	backGround = NULL;

	fallingObjs.clear();
	listKoopas.clear();
	// objs
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];
	objects.clear();

	/*delete mario;
	delete lugi;
	delete backGround;*/	
}

void CTitleScene::HandleMario()
{
	if (mario->x <= CCamera::GetInstance()->GetLeftMap() || mario->x >= CCamera::GetInstance()->GetRightMap() - 24)
		mario->vx = 0;

	if (mario->state == MARIO_STATE_DUCK && GetTickCount() - mario->duck_start > 300)
	{
		mario->SetState(MARIO_STATE_IDLE);
	}
}

void CTitleScene::HandleLugi()
{
	if (lugi->x <= CCamera::GetInstance()->GetLeftMap() || lugi->x >= CCamera::GetInstance()->GetRightMap() - 24)
	{
		lugi->vx = 0;
		lugi->x = CCamera::GetInstance()->GetRightMap() - 24;
	}

	if (lugi->state == MARIO_STATE_JUMP_HIGH && lugi->isOnGround && backGround->isScene3Begin)
	{
		lugi->SetState(MARIO_STATE_WALKING);
	}
}

void CTitleSceneKeyHandler::KeyState(BYTE* states)
{
}

void CTitleSceneKeyHandler::OnKeyDown(int KeyCode)
{
	switch (KeyCode)
	{
	case DIK_C:
		CGame::GetInstance()->SwitchScene(WORLD_MAP_1);
		break;
	case DIK_V:
		CGame::GetInstance()->SwitchScene(MAP_1_1);
		break;
	case DIK_UP:
		/*((CTitleScene*)scence)->SetPlayType(ONE_PLAYER_TYPE);*/
		((CTitleScene*)scence)->GetBackGround()->playType = ONE_PLAYER_TYPE;
		break;
	case DIK_DOWN:
		/*((CTitleScene*)scence)->SetPlayType(TWO_PLAYER_TYPE);*/
		((CTitleScene*)scence)->GetBackGround()->playType = TWO_PLAYER_TYPE;
		break;
	case DIK_W:
		switch (((CTitleScene*)scence)->GetBackGround()->playType)
		{
		case ONE_PLAYER_TYPE:
			CGame::GetInstance()->SwitchScene(WORLD_MAP_1);
			break;
		case TWO_PLAYER_TYPE:
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void CTitleSceneKeyHandler::OnKeyUp(int KeyCode)
{
}
