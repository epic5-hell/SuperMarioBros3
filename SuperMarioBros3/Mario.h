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

#define MARIO_STATE_IDLE				100
#define MARIO_STATE_WALKING_RIGHT		101
#define MARIO_STATE_WALKING_LEFT		102
#define MARIO_STATE_JUMP				103
#define MARIO_STATE_DIE					104


//#define MARIO_STATE_BRAKE_RIGHT			107	
//#define MARIO_STATE_BRAKE_LEFT			108

#define MARIO_STATE_RUNNING_RIGHT		130
#define MARIO_STATE_RUNNING_LEFT		140
#define MARIO_STATE_SPEED_DOWN			170
// BIG
#define MARIO_ANI_BIG_IDLE_RIGHT			0
#define MARIO_ANI_BIG_IDLE_LEFT				1
#define MARIO_ANI_BIG_WALKING_RIGHT			2
#define MARIO_ANI_BIG_WALKING_LEFT			3
#define MARIO_ANI_BIG_JUMPING_RIGHT			4
#define MARIO_ANI_BIG_JUMPING_LEFT			5
//#define MARIO_ANI_BIG_RUNNING_RIGHT		6
//#define MARIO_ANI_BIG_RUNNING_LEFT		7
//#define MARIO_ANI_BIG_RUNJUMPING_RIGHT	8
//#define MARIO_ANI_BIG_RUNJUMPING_LEFT		9
//#define MARIO_ANI_BIG_HOLDING_IDLE_RIGHT	10
//#define MARIO_ANI_BIG_HOLDING_RIGHT		11
//#define MARIO_ANI_BIG_HOLDING_IDLE_LEFT	12
//#define MARIO_ANI_BIG_HOLDING_LEFT		13
//#define MARIO_ANI_BIG_DUCKING_RIGHT		14
//#define MARIO_ANI_BIG_DUCKING_LEFT		15
//#define MARIO_ANI_BIG_KICKING_RIGHT		16
//#define MARIO_ANI_BIG_KICKING_LEFT		17
#define MARIO_ANI_BIG_BRAKING_RIGHT			6
#define MARIO_ANI_BIG_BRAKING_LEFT			7

// SMALL
#define MARIO_ANI_SMALL_IDLE_RIGHT		8
#define MARIO_ANI_SMALL_IDLE_LEFT		9
#define MARIO_ANI_SMALL_WALKING_RIGHT	10
#define MARIO_ANI_SMALL_WALKING_LEFT	11
#define MARIO_ANI_SMALL_JUMPING_RIGHT	12
#define MARIO_ANI_SMALL_JUMPING_LEFT	13
#define MARIO_ANI_SMALL_BRAKING_RIGHT	14
#define MARIO_ANI_SMALL_BRAKING_LEFT	15

// RACCOON


#define MARIO_ANI_RACCOON_BRAKING_RIGHT	19
#define MARIO_ANI_RACCOON_BRAKING_LEFT	20

#define MARIO_ANI_RACCOON_IDLE_RIGHT	13
#define MARIO_ANI_RACCOON_IDLE_LEFT		14
#define MARIO_ANI_RACCOON_WALKING_RIGHT	15
#define MARIO_ANI_RACCOON_WALKING_LEFT	16
#define MARIO_ANI_RACCOON_JUMPING_RIGHT	17
#define MARIO_ANI_RACCOON_JUMPING_LEFT	18
//#define MARIO_ANI_RACCOON_FALLING_RIGHT	19
//#define MARIO_ANI_RACCOON_FALLING_LEFT	20
//#define MARIO_ANI_RACCOON_RUNNING_RIGHT	21
//#define MARIO_ANI_RACCOON_RUNGING_LEFT	22
//#define MARIO_ANI_RACCOON_FLYING_RIGHT	23
//#define MARIO_ANI_RACCOON_FLYPING_LEFT	24
//#define MARIO_ANI_RACCOON_DUCKING_RIGHT	25
//#define MARIO_ANI_RACCOON_DUCKING_LEFT	26
//#define MARIO_ANI_RACCOON_SLIDING_RIGHT	25
//#define MARIO_ANI_RACCOON_SLIDING_LEFT	26
#define MARIO_ANI_RACCOON_SKIDDING_RIGHT	19
#define MARIO_ANI_RACCOON_SKIDDING_LEFT		20
//#define MARIO_ANI_RACCOON_HOLDING_RIGHT	25
//#define MARIO_ANI_RACCOON_HOLDING_LEFT	26
//#define MARIO_ANI_RACCOON_SPINNING_RIGHT	27
//#define MARIO_ANI_RACCOON_SPINNING_LEFT	28

#define MARIO_ANI_DIE					21

// FIRE
#define MARIO_ANI_FIRE_IDLE_RIGHT		24
#define MARIO_ANI_FIRE_IDLE_LEFT		25
#define MARIO_ANI_FIRE_WALKING_RIGHT	26
#define MARIO_ANI_FIRE_WALKING_LEFT		27
#define MARIO_ANI_FIRE_JUMPING_RIGHT	28
#define MARIO_ANI_FIRE_JUMPING_LEFT		29
#define MARIO_ANI_FIRE_BRAKING_RIGHT	30
#define MARIO_ANI_FIRE_BRAKING_LEFT		31




#define	MARIO_LEVEL_SMALL				1
#define	MARIO_LEVEL_BIG					2
#define	MARIO_LEVEL_RACCOON				3
#define	MARIO_LEVEL_FIRE				4

#define MARIO_BIG_BBOX_WIDTH			12
#define MARIO_BIG_BBOX_HEIGHT			27

#define MARIO_SMALL_BBOX_WIDTH			13
#define MARIO_SMALL_BBOX_HEIGHT			15

#define MARIO_RACCOON_BBOX_WIDTH		21
#define MARIO_RACCOON_BBOX_HEIGHT		27

#define MARIO_FIRE_BBOX_WIDTH			14
#define MARIO_FIRE_BBOX_HEIGHT			27

#define MARIO_UNTOUCHABLE_TIME			5000

#define MARIO_DIFFERENCE_HEIGHT			12


class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y;

	bool jump = false;
	bool brake;

public:
	bool IsReadyJump;
	bool IsTouchingGround;
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void Reset();
	void SetState(int _state);
	void SetLevel(int _level)
	{
		int oldLevel = this->level;
		this->level = _level;
		if (level == MARIO_LEVEL_SMALL)
		{
			y += MARIO_DIFFERENCE_HEIGHT;
		}
		else if (oldLevel == MARIO_LEVEL_SMALL)
		{
			y -= MARIO_DIFFERENCE_HEIGHT;
		}
	}
	int GetLevel() { return level; }

	bool GetJump()
	{
		return jump;
	}
	void SetJump(bool _jump)
	{
		this->jump = _jump;
	}
	
	bool GetBrake()
	{
		return brake;
	}
	void SetBrake(bool _brake)
	{
		brake = _brake;
		return;
	}

	void CalculatePotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);

	bool BrakeCalculation()
	{
		if (nx * vx < 0)
		{
			if (nx > 0)
			{
				vx += MARIO_WALKING_SPEED / 30;
			}
			else
			{
				vx -= MARIO_WALKING_SPEED / 30;
			}
			brake = true;
			return true;
		}
		else
		{
			brake = false;
		}

		return false;
	}

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};