#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "algorithm"
#include "PlayScence.h"

#define ALARM_BBOX_WIDTH			16
#define	ALARM_BBOX_HEIGHT			16

#define ALARM_BBOX_WIDTH_PRESSED	16
#define ALARM_BBOX_HEIGHT_PRESSED	7

#define ALARM_STATE_UNSHOW			100
#define ALARM_STATE_SHOW			200
#define ALARM_STATE_PRESSED			300

#define ALARM_ANI_SHOW		0
#define ALARM_ANI_PRESSED	1

class CAlarm : public CGameObject
{
private:
	bool active = false;
	bool appear = false;
public:
	CAlarm();
	virtual void Render();
	virtual void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void SetActive(bool _active)
	{
		this->active = _active;
	}
	bool GetActive()
	{
		return active;
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

