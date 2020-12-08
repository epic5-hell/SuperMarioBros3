#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScence.h"

#define BIG_COIN_BBOX_WIDTH		14
#define	BIG_COIN_BBOX_HEIGHT	16

#define	SMALL_COIN_BBOX_WIDTH	8
#define	SMALL_COIN_BBOX_HEIGHT	14


#define	BIG_COIN				100
#define	SMALL_COIN				200

#define	BIG_COIN_ANI_IDLE		0
#define	BIG_COIN_ANI_TURNING	1
#define	SMALL_COIN_ANI			2

class CCoin : public CGameObject
{
private:

	int type;
	bool appear;
	bool turning = false;
	bool rising;

	DWORD timing_start;

public:

	CCoin(int coin_type);
	virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);

	bool GetAppear()
	{
		return appear;
	}
	void SetAppear(bool _appear)
	{
		this->appear = _appear;
	}
	bool GetTurning()
	{
		return turning;
	}
	void SetTurning(bool _turning)
	{
		this->turning = _turning;
	}
};

