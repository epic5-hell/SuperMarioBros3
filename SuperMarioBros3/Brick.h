#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH	16
#define BRICK_BBOX_HEIGHT	16

#define BRICK_TYPE_NORMAL		100
#define BRICK_TYPE_BLOCK		200
//#define BRICK_TYPE_QUESTION		3

class CBrick : public CGameObject
{
private: 
	int type;

public:
	CBrick(int brick_type);
	
	virtual void Render();
	//virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	int getType() { return type; }
};