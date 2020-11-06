#pragma once
#include "GameObject.h"

#define MARIO_WALKING_SPEED				0.2f 
//0.1f
#define MARIO_SPEED_UP					0.008f
#define MARIO_SPEED_DOWN				0.01f
#define MARIO_SPEED_RUN					0.056f

#define MARIO_JUMP_SPEED_Y				0.5f
#define MARIO_JUMP_DEFLECT_SPEED		0.2f
#define MARIO_GRAVITY					0.002f
#define MARIO_DIE_DEFLECT_SPEED			0.5f

#define MARIO_STATE_IDLE				0
#define MARIO_STATE_WALKING_RIGHT		100
#define MARIO_STATE_WALKING_LEFT		200
#define MARIO_STATE_JUMP				300
#define MARIO_STATE_DIE					400

#define MARIO_STATE_BRAKE_RIGHT			700	
#define MARIO_STATE_BRAKE_LEFT			800

#define MARIO_STATE_RUNNING_RIGHT		1300
#define MARIO_STATE_RUNNING_LEFT		1400
#define MARIO_STATE_SPEED_DOWN			1700

#define MARIO_ANI_BIG_IDLE_RIGHT		0
#define MARIO_ANI_BIG_IDLE_LEFT			1
#define MARIO_ANI_BIG_WALKING_RIGHT		2
#define MARIO_ANI_BIG_WALKING_LEFT		3
#define MARIO_ANI_BIG_JUMPING_RIGHT		4
#define MARIO_ANI_BIG_JUMPING_LEFT		5
#define MARIO_ANI_BIG_BRAKING_RIGHT		8	
#define MARIO_ANI_BIG_BRAKING_LEFT		9


#define MARIO_ANI_SMALL_IDLE_RIGHT		20
#define MARIO_ANI_SMALL_IDLE_LEFT		21
#define MARIO_ANI_SMALL_WALKING_RIGHT	22
#define MARIO_ANI_SMALL_WALKING_LEFT	23
#define MARIO_ANI_SMALL_JUMPING_RIGHT	24
#define MARIO_ANI_SMALL_JUMPING_LEFT	25
#define MARIO_ANI_SMALL_BRAKING_RIGHT	28	
#define MARIO_ANI_SMALL_BRAKING_LEFT	29

#define MARIO_ANI_SMALL_WALKING_RIGHT	6
#define MARIO_ANI_SMALL_WALKING_LEFT	7

#define MARIO_ANI_DIE					8


#define	MARIO_LEVEL_SMALL				1
#define	MARIO_LEVEL_BIG					2

#define MARIO_BIG_BBOX_WIDTH			15
#define MARIO_BIG_BBOX_HEIGHT			27

#define MARIO_SMALL_BBOX_WIDTH			13
#define MARIO_SMALL_BBOX_HEIGHT			15

#define MARIO_UNTOUCHABLE_TIME			5000


class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y;

	bool IsJumping = false;
	bool brake;

public: 
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void Reset();
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void CalculatePotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);


	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	
};