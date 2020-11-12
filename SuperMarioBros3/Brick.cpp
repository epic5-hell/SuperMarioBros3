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
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y;
}