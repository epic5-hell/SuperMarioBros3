#pragma once
#include "GameObject.h"

#define MARIO_WALKING_SPEED				0.2f
#define MARIO_ACCELERATION				0.008f
#define MARIO_RUNNING_SPEED				0.056f 
#define MARIO_SPEED_DOWN				0.01f

//0.1f
//#define MARIO_SPEED_UP					0.008f
//#define MARIO_SPEED_DOWN				0.01f
//#define MARIO_SPEED_RUN					0.056f

#define MARIO_JUMP_SPEED_Y				0.5f
#define MARIO_JUMP_DEFLECT_SPEED		0.2f
#define MARIO_GRAVITY					0.002f
#define MARIO_DIE_DEFLECT_SPEED			0.5f

#define MARIO_STATE_IDLE				0
#define MARIO_STATE_WALKING_RIGHT		100
#define MARIO_STATE_WALKING_LEFT		200
#define MARIO_STATE_JUMP				300
#define MARIO_STATE_DIE					400
#define MARIO_STATE_FLYING_RIGHT		500
#define MARIO_STATE_FLYING_LEFT			600
#define MARIO_STATE_BRAKING_RIGHT		700	
#define MARIO_STATE_BRAKING_LEFT		800
#define MARIO_STATE_HOLDING_RIGHT		900
#define MARIO_STATE_HOLDING_LEFT		1000
#define MARIO_STATE_HOLDING				1100
#define MARIO_STATE_KICK				1200
#define MARIO_STATE_RUNNING_RIGHT		1300
#define MARIO_STATE_RUNNING_LEFT		1400
#define MARIO_STATE_SITDOWN				1500
#define MARIO_STATE_TURNING_TAIL		1600
#define MARIO_STATE_SPEED_DOWN			1700
#define MARIO_STATE_FALLING_DOWN		1800



// opportunity: idle, walking, jumping, braking, running, hold idle, hold right, hold left, kicking, sit down, spin

// BIG
#define MARIO_ANI_BIG_IDLE_RIGHT				0
#define MARIO_ANI_BIG_IDLE_LEFT					1
#define MARIO_ANI_BIG_WALKING_RIGHT				2
#define MARIO_ANI_BIG_WALKING_LEFT				3
#define MARIO_ANI_BIG_JUMPING_RIGHT				4
#define MARIO_ANI_BIG_JUMPING_LEFT				5
#define MARIO_ANI_BIG_BRAKING_RIGHT				6
#define MARIO_ANI_BIG_BRAKING_LEFT				7
#define MARIO_ANI_BIG_RUNNING_RIGHT				8
#define MARIO_ANI_BIG_RUNNING_LEFT				9
#define MARIO_ANI_BIG_RUNNING_MAX_SPEED_RIGHT	10
#define MARIO_ANI_BIG_RUNNING_MAX_SPEED_LEFT	11
#define MARIO_ANI_BIG_HOLDING_RIGHT				12
#define MARIO_ANI_BIG_HOLDING_LEFT				13
#define MARIO_ANI_BIG_KICKING_RIGHT				14
#define MARIO_ANI_BIG_KICKING_LEFT				15
#define	MARIO_ANI_BIG_SIT_DOWN_RIGHT			16
#define MARIO_ANI_BIG_SIT_DOWN_LEFT				17


// SMALL
#define MARIO_ANI_SMALL_IDLE_RIGHT				18
#define MARIO_ANI_SMALL_IDLE_LEFT				19
#define MARIO_ANI_SMALL_WALKING_RIGHT			20
#define MARIO_ANI_SMALL_WALKING_LEFT			21
#define MARIO_ANI_SMALL_JUMPING_RIGHT			22
#define MARIO_ANI_SMALL_JUMPING_LEFT			23
#define MARIO_ANI_SMALL_BRAKING_RIGHT			24
#define MARIO_ANI_SMALL_BRAKING_LEFT			25
#define MARIO_ANI_SMALL_RUNNING_RIGHT			26
#define MARIO_ANI_SMALL_RUNNING_LEFT			27
#define MARIO_ANI_SMALL_RUNNING_MAX_SPEED_RIGHT 28
#define	MARIO_ANI_SMALL_RUNNING_MAX_SPEED_LEFT	29
#define MARIO_ANI_SMALL_HOLDING_RIGHT			30
#define MARIO_ANI_SMALL_HOLDING_LEFT			31
#define MARIO_ANI_SMALL_KICKING_RIGHT			32
#define MARIO_ANI_SMALL_KICKING_LEFT			33
#define	MARIO_ANI_SMALL_SIT_DOWN_RIGHT			34
#define MARIO_ANI_SMALL_SIT_DOWN_LEFT			35

// RACCOON
#define MARIO_ANI_RACCOON_IDLE_RIGHT				36
#define MARIO_ANI_RACCOON_IDLE_LEFT					37
#define MARIO_ANI_RACCOON_WALKING_RIGHT				38
#define MARIO_ANI_RACCOON_WALKING_LEFT				39
#define MARIO_ANI_RACCOON_JUMPING_RIGHT				40
#define MARIO_ANI_RACCOON_JUMPING_LEFT				41
#define MARIO_ANI_RACCOON_BRAKING_RIGHT				42
#define MARIO_ANI_RACCOON_BRAKING_LEFT				43
#define MARIO_ANI_RACCOON_RUNNING_RIGHT				44
#define MARIO_ANI_RACCOON_RUNNING_LEFT				45
#define	MARIO_ANI_RACCOON_RUNNING_MAX_SPEED_RIGHT	46
#define MARIO_ANI_RACCOON_RUNNING_MAX_SPEED_LEFT	47
#define MARIO_ANI_RACCOON_HOLDING_RIGHT				48
#define MARIO_ANI_RACCOON_HOLDING_LEFT				49
#define MARIO_ANI_RACCOON_KICKING_RIGHT				50
#define MARIO_ANI_RACCOON_KICKING_LEFT				51
#define	MARIO_ANI_RACCOON_SIT_DOWN_RIGHT			52
#define MARIO_ANI_RACCOON_SIT_DOWN_LEFT				53
#define MARIO_ANI_RACCOON_FLYING_RIGHT				54
#define MARIO_ANI_RACCOON_FLYING_LEFT				55
#define MARIO_ANI_RACCOON_FALLING_RIGHT				56
#define MARIO_ANI_RACCOON_FALLING_LEFT				57
#define MARIO_ANI_RACCOON_SPINNING_RIGHT			58
#define MARIO_ANI_RACCOON_SPINNING_LEFT				59

#define MARIO_ANI_DIE								60

// FIRE
#define MARIO_ANI_FIRE_IDLE_RIGHT				37
#define MARIO_ANI_FIRE_IDLE_LEFT				38
#define MARIO_ANI_FIRE_WALKING_RIGHT			39
#define MARIO_ANI_FIRE_WALKING_LEFT				40
#define MARIO_ANI_FIRE_JUMPING_RIGHT			41
#define MARIO_ANI_FIRE_JUMPING_LEFT				42
#define MARIO_ANI_FIRE_BRAKING_RIGHT			43
#define MARIO_ANI_FIRE_BRAKING_LEFT				44
#define MARIO_ANI_FIRE_RUNNING_RIGHT			45
#define	MARIO_ANI_FIRE_RUNNING_LEFT				46
#define MARIO_ANI_FIRE_RUNNING_MAX_SPEED_RIGHT	47
#define	MARIO_ANI_FIRE_RUNNING_MAX_SPEED_LEFT	48
#define MARIO_ANI_FIRE_HOLDING_RIGHT			48
#define MARIO_ANI_FIRE_HOLDING_LEFT			49
#define MARIO_ANI_FIRE_KICKING_RIGHT			50
#define MARIO_ANI_FIRE_KICKING_LEFT			51
#define	MARIO_ANI_FIRE_SIT_DOWN_RIGHT			52
#define MARIO_ANI_FIRE_SIT_DOWN_LEFT			53
#define MARIO_ANI_FIRE_SHOOTING_RIGHT			78	
#define MARIO_ANI_FIRE_SHOOTING_LEFT			79

#define	MARIO_LEVEL_SMALL				1
#define	MARIO_LEVEL_BIG					2
#define	MARIO_LEVEL_RACCOON				3
#define	MARIO_LEVEL_FIRE				4

#define MARIO_BIG_BBOX_WIDTH			15
#define MARIO_BIG_BBOX_HEIGHT			27

#define MARIO_SMALL_BBOX_WIDTH			13
#define MARIO_SMALL_BBOX_HEIGHT			15

#define MARIO_RACCOON_BBOX_WIDTH		21
#define MARIO_RACCOON_BBOX_HEIGHT		27

#define MARIO_FIRE_BBOX_WIDTH			14
#define MARIO_FIRE_BBOX_HEIGHT			27

#define MARIO_UNTOUCHABLE_TIME			5000

#define MARIO_DIFFERENCE_HEIGHT			12

// run
#define MARIO_RUNNING_LIMIT_TIME		300
//spin
#define MARIO_SPINNING_TIME				400
//kick
#define MARIO_KICKING_TIME				200	
//shoot
#define MARIO_FIRING_TIME				400	
#define MARIO_RUNNING_LIMIT_TIME		300
#define MARIO_FLYING_LIMIT_TIME			7000
#define MARIO_MAX_STACK					7


class CMario : public CGameObject
{
	int level;
	int untouchable;
	int time_mario = 0;
	DWORD untouchable_start;
	DWORD running_start = 0;
	DWORD turning_start = 0;
	DWORD flying_start = 0;

	float start_x;			// initial position of Mario at scene
	float start_y;
	bool jumping = false;
	bool flying = false;

	bool brake;

	bool canFly = false;
	
public:
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	void CalculatePotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
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

	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	//run
	void StartRunning() { running_start = GetTickCount(); }
	//fly
	void StartFlying() { flying_start = GetTickCount(); }

	void Reset();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	// jump
	bool GetJumping()
	{
		return jumping;
	}
	void SetJumping(bool _jumping)
	{
		this->jumping = _jumping;
	}
	
	//brake
	bool GetBraking()
	{
		return brake;
	}
	void SetBraking(bool _brake)
	{
		brake = _brake;
		return;
	}
	bool BrakeCalculation()
	{
		if (nx * vx < 0)
		{
			if (nx > 0)
			{
				vx += MARIO_WALKING_SPEED / 10;
			}
			else
			{
				vx -= MARIO_WALKING_SPEED / 10;
			}
			brake = true;
			time_mario = 0;
			return true;
		}
		else
		{
			brake = false;
		}

		return false;
	}

	//run
	DWORD GetRunningStart()
	{
		return running_start;
	}
	void CalculateMarioTimeUp()
	{
		if (GetTickCount() - running_start > MARIO_RUNNING_LIMIT_TIME && time_mario <= MARIO_MAX_STACK)
		{
			running_start = 0;
			time_mario += 1;
		}
	}
	void SetMarioTime(int _time_mario)
	{
		time_mario = _time_mario;
	}

	//fly
	DWORD GetFlyingStart()
	{
		return flying_start;
	}
	bool GetFlying()
	{
		return flying;
	}
	void SetFlying(int _flying)
	{
		this->flying = _flying;
	}
	bool GetCanFly()
	{
		return canFly;
	}
	void SetCanFly(bool _canFly)
	{
		canFly = _canFly;
		return;
	}
};