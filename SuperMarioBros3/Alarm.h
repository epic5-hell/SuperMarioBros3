#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "algorithm"
#include "PlayScence.h"

#define ALARM_BBOX_WIDTH			16
#define	ALARM_BBOX_HEIGHT			16

#define ALARM_STATE_UNSHOW			100
#define ALARM_STATE_SHOW			200
#define ALARM_STATE_PRESSED			300

#define ALARM_ANI_SHOW		0
#define ALARM_ANI_ACTIVE	1

class CAlarm : public CGameObject
{
private:
	bool activate = false;
	bool appear = false;
public:
	CAlarm();
	virtual void Render();
	virtual void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void SetActivate(bool _activate)
	{
		this->activate = _activate;
	}
	bool GetActivate()
	{
		return activate;
	}
	void SetAppear(bool _appear)
	{
		this->appear = _appear;
	}
	bool GetAppear()
	{
		return appear;
	}
};

