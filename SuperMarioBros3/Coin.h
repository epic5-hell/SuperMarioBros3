#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScence.h"

#define SMALL_COIN_DEFLECT_SPEED	0.2f

#define BIG_COIN_BBOX_WIDTH		14
#define	BIG_COIN_BBOX_HEIGHT	16

#define	SMALL_COIN_BBOX_WIDTH	8
#define	SMALL_COIN_BBOX_HEIGHT	14


#define	BIG_COIN				100
#define	SMALL_COIN				200

#define	BIG_COIN_ANI			0
#define	SMALL_COIN_ANI			1

class CCoin : public CGameObject
{
private:

	int type;
	bool appear;

	bool coin_up;
	bool coin_down;
	DWORD up_down = 0;;

public:

	CCoin(int coin_type);
	virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	void StartUpDown()
	{
		up_down = GetTickCount64();
	}

	int GetType()
	{
		return type;
	}
	bool GetAppear()
	{
		return appear;
	}
	void SetAppear(bool _appear)
	{
		this->appear = _appear;
	}
	bool GetCoinUp()
	{
		return coin_up;
	}
	void SetCoinUp(bool _coin_up)
	{
		this->coin_up = _coin_up;
	}
	bool GetCoinDown()
	{
		return coin_down;
	}
	void SetCoinDown(bool _coin_down)
	{
		this->coin_down = _coin_down;
	}
};

