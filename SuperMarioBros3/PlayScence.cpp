#include <iostream>
#include <fstream>

#include "PlayScence.h"


using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):CScene(id, filePath)
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

#define OBJECT_TYPE_MARIO				0
#define OBJECT_TYPE_BRICK				1
#define OBJECT_TYPE_BLOCK				2
#define OBJECT_TYPE_BRICk_REWARD		3
#define OBJECT_TYPE_NORMAL_GOOMBA		4
#define	OBJECT_TYPE_WINGS_GOOMBA		5
#define OBJECT_TYPE_GREEN_KOOPAS_WALK	6
#define OBJECT_TYPE_GREEN_KOOPAS_WINGS	7


#define OBJECT_TYPE_PORTAL				50

#define MAX_SCENE_LINE					1024


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

void CPlayScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 7) return; // skip invalid lines

	int idTileSet = atoi(tokens[0].c_str());
	int totalRowsTileSet = atoi(tokens[1].c_str());
	int totalColumnsTileSet = atoi(tokens[2].c_str());
	int totalRowsMap = atoi(tokens[3].c_str());
	int totalColumnsMap = atoi(tokens[4].c_str());
	int totalTiles = atoi(tokens[5].c_str());
	wstring file_path = ToWSTR(tokens[6]);

	this->map = new Map(idTileSet, totalRowsTileSet, totalColumnsTileSet, totalRowsMap, totalColumnsMap, totalTiles);
	map->LoadMap(file_path.c_str());
	map->ExtractTileFromTileSet();
}

// [SPRITES]
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

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}
// [ANIMATIONS]
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
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}
// [ANIMATION_SETS]
void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

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

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x,y); 
		player = (CMario*)obj;  

		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(100); break;
	case OBJECT_TYPE_BLOCK:	obj = new CBrick(200); break;
	case OBJECT_TYPE_NORMAL_GOOMBA: obj = new CGoomba(888); break;
	//case OBJECT_TYPE_RED_KOOPAS: obj = new CKoopas(111); break;
	case OBJECT_TYPE_GREEN_KOOPAS_WALK: obj = new CKoopas(111); break;
	/*case OBJECT_TYPE_PORTAL:
		{	
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			int scene_id = atoi(tokens[6].c_str());
			obj = new CPortal(x, y, r, b, scene_id);
		}
		break;*/
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	/*if (sceneFilePath == nullptr) return;*/
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[MAP]") { section = SCENE_SECTION_MAP; continue; }
		if (line == "[SPRITES]") { section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; }
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
			case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
			case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
			case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
			case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
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
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame *game = CGame::GetInstance();
	float ScreenWidth = game->GetScreenWidth();
	float ScreenHeight = game->GetScreenHeight();
	float MapWidth = map->GetMapWidth();
	float MapHeight = map->GetMapHeight();

	// update camera following to mario
	if (map != nullptr && (cx > MapWidth - ScreenWidth / 2))
		cx = MapWidth - ScreenWidth;
	else if (cx < ScreenWidth / 2)
		cx = 0;
	else
		cx -= ScreenWidth / 2;

	if (map != nullptr && (cy > MapHeight - ScreenHeight / 2))
		cy = MapHeight - ScreenHeight;
	else if (cy < ScreenHeight / 2)
		cy = 0;
	else
		cy -= ScreenHeight / 2;	

	CGame::GetInstance()->SetCamPos(round(cx), round(cy));
}

void CPlayScene::Render()
{
	if (map)
	{
		this->map->Render();
	}
		
	for (int i = 0; i < objects.size(); i++)
	{
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

	delete map;
	map = nullptr;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->GetState() == MARIO_STATE_DIE) return;

	switch (KeyCode)
	{
	case DIK_SPACE:
		if (mario->GetJumping() == false)
		{
			mario->SetState(MARIO_STATE_JUMP);
			mario->SetJumping(true);
		}
		break;
	case DIK_A:
		mario->Reset();
		break;
	case DIK_W:
		if (mario->GetLevel() == MARIO_LEVEL_SMALL) return;
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_E:
		if (mario->GetLevel() == MARIO_LEVEL_BIG) return;
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_R:
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON) return;
		mario->SetLevel(MARIO_LEVEL_RACCOON);
		break;
	case DIK_T:
		if (mario->GetLevel() == MARIO_LEVEL_FIRE) return;
		mario->SetLevel(MARIO_LEVEL_FIRE);
		break;
	case DIK_Z:
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON && mario->GetTurning() == false)
		{
			mario->StartTurning();
			mario->SetState(MARIO_STATE_TURNING_TAIL);
			mario->SetTurning(true);
		}
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->GetState() == MARIO_STATE_DIE) return;
	switch (KeyCode)
	{
	case DIK_Q:
		mario->SetHolding(false);
		mario->SetCanHold(false);
		break;
	case DIK_X:
		return;
		/*mario->SetCanFly(false);
		mario->SetFlying(false);
		mario->SetFalling(true);
		mario->SetJumping(true);*/
		break;
	}
}


void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();

	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DIE) return;

	//holding koopas shell
	if (game->IsKeyDown(DIK_Q))
	{
		mario->SetHolding(true);
	}

	else if (game->IsKeyDown(DIK_X))
	{
		return;
		//if (mario->GetMarioTime() >= MARIO_MAX_STACK)
		//	mario->SetCanFly(true);
		//if (/*mario->GetLevel() == MARIO_LEVEL_RACCOON &&*/ mario->GetCanFly())
		//{
		//	if (mario->nx > 0)
		//	{
		//		mario->SetState(MARIO_STATE_FLYING_RIGHT);
		//	}
		//	else if (mario->nx < 0)
		//	{
		//		mario->SetState(MARIO_STATE_FLYING_LEFT);
		//	}
		//	if (mario->GetFlyingStart() == 0)
		//	{
		//		mario->StartFlying();
		//	}
		//	mario->SetFlying(true);
		//}
		//else
		//{
		//	if (mario->GetCanFall() == true)
		//	{
		//		mario->SetState(MARIO_STATE_FALLING_DOWN);
		//		mario->SetFalling(true);
		//	}
		//}
	}
	// moving
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_LSHIFT)) //running right
		{
			if (mario->GetRunningStart() == 0)
			{
				mario->StartRunning();
			}
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
			mario->CalcMarioTimeUp();
		}
		else // just walking right
		{
			mario->SetMarioTime(0);
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
		}
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_LSHIFT)) //running left
		{
			if (mario->GetRunningStart() == 0)
			{
				mario->StartRunning();
			}
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
			mario->CalcMarioTimeUp();
		}
		else // just walking left
		{
			mario->SetMarioTime(0);
			mario->SetState(MARIO_STATE_WALKING_LEFT);
		}
	}
	else if (game->IsKeyDown(DIK_DOWN))    //sit down
	{
		if (mario->GetLevel() != MARIO_LEVEL_SMALL)
		{
			mario->SetState(MARIO_STATE_SITDOWN);
		}
	}
	else
	{
		mario->SetMarioTime(0);
		if ((mario->nx > 0 && mario->vx <= 0) || (mario->nx < 0 && mario->vx >= 0))
		{
			mario->SetState(MARIO_STATE_IDLE);
		}
		if (mario->vx != 0)
		{
			mario->SetState(MARIO_STATE_SPEED_DOWN);
		}
	}

}