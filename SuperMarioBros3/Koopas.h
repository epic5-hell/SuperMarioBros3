#pragma once

#include "GameObject.h"
#include "Mario.h"
#include "Goomba.h"
#include "PlayScence.h"
#include "Scence.h"

#define KOOPAS_WALKING_SPEED		0.03f
#define KOOPAS_GRAVITY				0.002f
#define KOOPAS_DIE_DEFLECT_SPEED	0.5f
#define KOOPAS_BBOX_WIDTH			16
#define KOOPAS_BBOX_HEIGHT			26
#define KOOPAS_BBOX_SHELL_HEIGHT	16
#define KOOPAS_BBOX_HEIGHT_DIE		16

#define KOOPAS_STATE_WALKING		100
#define KOOPAS_STATE_SHELL			200
#define KOOPAS_STATE_DIE			300
#define	KOOPAS_STATE_SPINNING		400
#define	KOOPAS_STATE_HOLDING		500

#define KOOPAS_GREEN_ANI_WALKING_RIGHT	0
#define KOOPAS_GREEN_ANI_WALKING_LEFT	1
//#define KOOPAS_GREEN_ANI_FLYING		2
#define KOOPAS_GREEN_ANI_SHELL_UP		2
#define KOOPAS_GREEN_ANI_SHELL_DOWN		3
#define KOOPAS_GREEN_ANI_SHELL_SPIN_RIGHT	4
#define KOOPAS_GREEN_ANI_SHELL_SPIN_LEFT	5

#define KOOPAS_JUMP_SPEED			0.35f
#define KOOPAS_TIME_JUMPING			900

#define KOOPAS_TYPE_GREEN_WALK			111
#define KOOPAS_TYPE_GREEN_FLY			222


class CKoopas : public CGameObject
{
	int type_koopas;
	bool holding = false;


public:
	CKoopas(int _type_koopas);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);

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
		return type_koopas;
	}
	void SetType(int typeInt)
	{
		type_koopas = typeInt;
	}
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
};