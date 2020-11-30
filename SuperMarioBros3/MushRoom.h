#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "PlayScence.h"

#define MUSHROOM_BBOOX_WIDTH	16
#define MUSHROOM_BBOX_HEIGHT	16

#define MUSHROOM_SPEED_MOVING	0.04f
#define	MUSHROOM_SPEED_RISING	-0.08f

#define MUSHROOM_STATE_IDLE		100
#define MUSHROOM_STATE_RISING	200
#define MUSHROOM_STATE_MOVING	300

#define MUSHROOM_GRAVITY		0.002f
#define MUSHROOM_RED			111
#define MUSHROOM_GREEN			222

#define MUSHROOM_ANI_RED		0
#define MUSHROOM_ANI_GREEN		1


class CMushRoom:public CGameObject
{
private:
	int type;
	bool appear = false;
	DWORD rising_start = 0;
public:
	CMushRoom(int mushroom_type);

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void SetState(int state);
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
	
	void StartRising() { rising_start = GetTickCount64(); }
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
		return;
	}
};

