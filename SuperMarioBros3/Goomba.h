#pragma once
#include "GameObject.h"
#include "algorithm"

#define GOOMBA_WALKING_SPEED		0.05f
#define GOOMBA_GRAVITY				0.002f
#define GOOMBA_DIE_DEFLECT_SPEED	0.5f
#define GOOMBA_JUMP_SPEED			0.35f

#define GOOMBA_NORMAL_BBOX_WIDTH	16
#define GOOMBA_NORMAL_BBOX_HEIGHT	15
#define GOOMBA_WINGS_BBOX_WIDTH		20
#define GOOMBA_WINGS_BBOX_HEIGHT	24
#define GOOMBA_GOOMBA_LOSE_WINGS_BBOX_HEIGHT	19
#define GOOMBA_BBOX_HEIGHT_DIE		9

#define GOOMBA_STATE_WALKING		100
#define GOOMBA_STATE_DIE			200
#define GOOMBA_STATE_DISAPPEAR		300
#define GOOMBA_STATE_DIE_BY_KICK	400

#define GOOMBA_NORMAL_ANI_WALKING	0
#define GOOMBA_NORMAL_ANI_DIE		1

#define GOOMBA_TYPE_NORMAL				888
#define GOOMBA_TYPE_WINGS				999

class CGoomba : public CGameObject
{

	int type_goomba;


public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	CGoomba(int _type_goomba);
	int GetType()
	{
		return type_goomba;
	}
	virtual void SetState(int state);
};