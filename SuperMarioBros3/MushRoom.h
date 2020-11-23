#pragma once
#include "GameObject.h"



class CMushRoom:public CGameObject
{
private:
	int type;
	bool appear = false;
	DWORD rising_start = 0;
public:
	CMushRoom();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state) { this->state = state; }
};

