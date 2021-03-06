#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "PlayScence.h"

#define BRICK_BBOX_WIDTH	16
#define BRICK_BBOX_HEIGHT	16

#define BREAKABLE_BRICK_BBOX_WIDTH	16
#define BREAKABLE_BRICK_BBOX_HEIGHT	16

#define BRICK_BIG_COIN_BBOX_WIDTH	14
#define	BRICK_BIG_COIN_BBOX_HEIGHT	10

#define BREAKABLE_BRICK_STATE_NORMAL	1000
#define BREAKABLE_BRICK_STATE_BREAK		2000
#define BREAKABLE_BRICK_STATE_COIN		3000

#define BRICK_TYPE_NORMAL					100
#define BRICK_TYPE_BLOCK					200
#define BRICK_TYPE_QUESTION_NORMAL			300
#define BRICK_TYPE_QUESTION_MUSHROOM_LEAF	400 // red mushroom and leaf
#define BRICK_TYPE_NEW						500
#define BRICK_TYPE_BREAKABLE				600

#define BRICK_QUESTION_ANI_ALIVE		0
#define BRICK_QUESTION_ANI_DEAD			1
#define BREAKABLE_BRICK_ANI_NORMAL		2
#define BREAKABLE_BRICK_ANI_COIN		3

#define TIME_BREAKABLE_BRICK_REVIVE		100000

class CBrick : public CGameObject
{
private:
	int type;
	bool alive = true;
	bool used = false;

	bool show_brick = true;
	bool revive = false;
	bool break_brick = false;
	DWORD reviving_start = 0;
	

public:
	CBrick(int brick_type);
	virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void SetState(int state);
	int GetType() { return type; }

	// question brick
	bool GetAlive()
	{
		return alive;
	}
	void SetAlive(bool _alive)
	{
		this->alive = _alive;
	}
	bool GetUsed()
	{
		return used;
	}
	void SetUsed(bool _used)
	{
		this->used = _used;
	}

	// breakable brick
	bool GetShowBrick()
	{
		return show_brick;
	}
	void SetShowBrick(bool _show_brick)
	{
		this->show_brick = _show_brick;
	}
	bool GetRevive()
	{
		return revive;
	}
	void SetRevive(bool _revive)
	{
		this->revive = _revive;
	}
	void StartRevive()
	{
		reviving_start = GetTickCount64();
	}
	bool GetBreakBrick()
	{
		return break_brick;
	}
	void SetBreakBrick(bool _break_brick)
	{
		this->break_brick = _break_brick;
	}
};