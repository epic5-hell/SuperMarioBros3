#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

#define BRICK_TYPE_NORMAL		 0
#define BRICK_TYPE_BIG			 1
#define BRICK_TYPE_QUESTION_MARK 2

class CBrick : public CGameObject
{
private: 
	int type;

public:
	CBrick(int _type = 0);
	~CBrick();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetTypeBrick() { return this->type; }
};