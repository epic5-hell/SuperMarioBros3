#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "PlayScence.h"

#define BRICK_BBOX_WIDTH	16
#define BRICK_BBOX_HEIGHT	16

#define BREAKABLE_BRICK_STATE_NORMAL	1000
#define BREAKABLE_BRICK_STATE_BREAK		2000
#define BREAKABLE_BRICK_STATE_COIN		3000

#define BRICK_TYPE_NORMAL					100
#define BRICK_TYPE_BLOCK					200
#define BRICK_TYPE_QUESTION_NORMAL			300
#define BRICK_TYPE_QUESTION_MUSHROOM_LEAF	400 // red mushroom and leaf
#define BRICK_TYPE_QUESTION_GREEN_MUSHROOM	500
#define BRICK_TYPE_BREAKABLE				600

#define BRICK_QUESTION_ANI_ALIVE		0
#define BRICK_QUESTION_ANI_DEAD			1
#define BREAKABLE_BRICK_ANI_NORMAL		2
#define BREAKABLE_BRICK_ANI_COIN		3

class CBrick : public CGameObject
{
private:
	int type;
	bool alive = true;
	bool used = false;

public:
	CBrick(int brick_type);
	virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);

	int GetType() { return type; }

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
};