#include "Brick.h"

CBrick::CBrick(int brick_type)
{
	type = brick_type;
}
void CBrick::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (type == BRICK_TYPE_BLOCK)
	{
		l = x;
		t = y;
		r = x + BRICK_BBOX_WIDTH;
		b = y;
	}
	else // BRICK_TYPE_NORMAL
	{
		l = x;
		t = y;
		r = x + BRICK_BBOX_WIDTH;
		b = y+BRICK_BBOX_HEIGHT;
	}
}