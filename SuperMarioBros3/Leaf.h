#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "PlayScence.h"

#define LEAF_BBOX_WIDTH		16
#define LEAF_BBOX_HEIGHT	14

#define LEAF_STATE_IDLE			100
#define LEAF_STATE_RISING		200
#define LEAF_STATE_SETTING		300

#define LEAF_ANI_RIGHT		0
#define LEAF_ANI_LEFT		1

class CLeaf : public CGameObject
{
private:

	bool appear = false;
	DWORD rising_start;
	DWORD setting_start = 0;

public:

	CLeaf();

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
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
	void StartSetting() { setting_start = GetTickCount64(); }

	bool GetAppear()
	{
		return appear;
	}

	void SetAppear(bool _appear)
	{
		this->appear = _appear;
	}
};
