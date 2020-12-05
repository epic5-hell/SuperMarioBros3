#pragma once

#include "GameObject.h"
#include "Mario.h"
#include "Goomba.h"
#include "PlayScence.h"
#include "Scence.h"

#define KOOPAS_WALKING_SPEED		0.03f
#define KOOPAS_GRAVITY				0.002f
#define KOOPAS_DIE_DEFLECT_SPEED	0.5f
#define KOOPAS_SHELL_DEFLECT_SPEED	0.4f
#define KOOPAS_BBOX_WIDTH			16
#define KOOPAS_BBOX_HEIGHT			26
#define KOOPAS_BBOX_SHELL_HEIGHT	16
#define KOOPAS_BBOX_HEIGHT_DIE		16

#define KOOPAS_STATE_WALKING		100
#define KOOPAS_STATE_SHELL			200
#define KOOPAS_STATE_DIE			300
#define	KOOPAS_STATE_SPINNING		400
#define	KOOPAS_STATE_HOLDING		500

#define KOOPAS_GREEN_ANI_WALKING_RIGHT			0
#define KOOPAS_GREEN_ANI_WALKING_LEFT			1
#define KOOPAS_GREEN_ANI_SHELL_UP_IDLE			2
#define KOOPAS_GREEN_ANI_SHELL_DOWN_IDLE		3
#define KOOPAS_GREEN_ANI_SHELL_DOWN_SPIN_RIGHT	4
#define KOOPAS_GREEN_ANI_SHELL_DOWN_SPIN_LEFT	5
#define KOOPAS_GREEN_ANI_SHELL_UP_SPIN_RIGHT	6
#define KOOPAS_GREEN_ANI_SHELL_UP_SPIN_LEFT		7
#define KOOPAS_GREEN_ANI_REVIVE_DOWN			8
#define KOOPAS_GREEN_ANI_REVIVE_UP				9
#define KOOPAS_GREEN_ANI_FLYING_RIGHT				10
#define KOOPAS_GREEN_ANI_FLYING_LEFT				11
#define KOOPAS_RED_ANI_WALKING_RIGHT			12
#define KOOPAS_RED_ANI_WALKING_LEFT				13
#define KOOPAS_RED_ANI_SHELL_UP_IDLE			14
#define KOOPAS_RED_ANI_SHELL_DOWN_IDLE			15
#define KOOPAS_RED_ANI_SHELL_DOWN_SPIN_RIGHT	16
#define KOOPAS_RED_ANI_SHELL_DOWN_SPIN_LEFT		17
#define KOOPAS_RED_ANI_SHELL_UP_SPIN_RIGHT		18
#define KOOPAS_RED_ANI_SHELL_UP_SPIN_LEFT		19
#define KOOPAS_GREEN_ANI_REVIVE_DOWN			20
#define KOOPAS_GREEN_ANI_REVIVE_UP				21

#define KOOPAS_JUMP_SPEED			0.35f
#define KOOPAS_TIME_JUMPING			700

#define KOOPAS_TYPE_GREEN_WALK			111
#define KOOPAS_TYPE_GREEN_WINGS			222
#define	KOOPAS_TYPE_RED_WALK			333


class CKoopas : public CGameObject
{
private:

	int type;
	float past_y;

	bool holding = false;
	bool CanPullBack = false;
	bool ShellUp = false;

	DWORD jumping_start = 0;
	
public:
	CKoopas(int _type_koopas);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny,
		float& rdx,
		float& rdy);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	
	void StartJumping() { jumping_start = GetTickCount64(); }

	//being hled
	bool GetHolding()
	{
		return holding;
	}
	void SetHolding(bool _holding)
	{
		this->holding = _holding;
	}
	int GetType()
	{
		return type;
	}
	void SetType(int koopas_type)
	{
		type = koopas_type;
	}
	// shell up
	bool GetShellUp()
	{
		return ShellUp;
	}
	void SetShellUp(bool _ShellUp)
	{
		this->ShellUp = _ShellUp;
	}

};