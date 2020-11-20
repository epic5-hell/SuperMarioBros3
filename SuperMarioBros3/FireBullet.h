#pragma once
#include "GameObject.h"
#include "PlayScence.h"
#include "Mario.h"
#include <algorithm>


#define BULLET_BBOX_WIDTH	9
#define BULLET_BBOX_HEIGHT	9

#define BULLET_GRAVITY		0.0009f
#define BULLET_FLYING_SPEED	0.1f

#define BULLET_STATE_FLYING		100
#define	BULLET_STATE_DISAPPEAR	200

#define BULLET_ANI_FLYING_RIGHT		0
#define BULLET_ANI_FLYING_LEFT		1

class CFireBullet: public CGameObject
{
	bool IsUsed = false;
	float height_limit;

public:
	CFireBullet();

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);

	bool GetIsUsed()
	{
		return IsUsed;
	}

	void SetIsUsed(bool IsUsed)
	{
		this->IsUsed = IsUsed;
	}
};

