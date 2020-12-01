#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "Koopas.h"

#define GOOMBA_WALKING_SPEED		0.05f
#define GOOMBA_GRAVITY				0.002f
#define GOOMBA_DIE_DEFLECT_SPEED	0.5f
#define GOOMBA_JUMP_SPEED			0.35f

#define GOOMBA_NORMAL_BBOX_WIDTH	16
#define GOOMBA_NORMAL_BBOX_HEIGHT	15
#define GOOMBA_WINGS_BBOX_WIDTH		20
#define GOOMBA_WINGS_BBOX_HEIGHT	24
#define GOOMBA_LOSE_WINGS_BBOX_WIDTH	20
#define GOOMBA_LOSE_WINGS_BBOX_HEIGHT	19


#define GOOMBA_STATE_WALKING		100
#define GOOMBA_STATE_DIE			200
#define GOOMBA_STATE_DISAPPEAR		300
#define GOOMBA_STATE_DIE_BY_KICK	400
#define GOOMBA_STATE_LOSE_WINGS		500

#define GOOMBA_NORMAL_ANI_WALKING	0
#define GOOMBA_NORMAL_ANI_DIE		1

#define GOOMBA_TYPE_NORMAL				888
#define GOOMBA_TYPE_WINGS				999

class CGoomba : public CGameObject
{
	int type;
	DWORD dying_start = 0;

public:
	CGoomba(int goomba_type);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	
	void StartDying() { dying_start = GetTickCount64(); }
	int GetType()
	{
		return type;
	}
	virtual void SetState(int state);
};