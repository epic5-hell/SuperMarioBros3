#include "Brick.h"

CBrick::CBrick(int brick_type)
{
	type = brick_type;
}
void CBrick::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (dynamic_cast<CBrick*>(coObjects->at(i)))
		{
			CBrick* brick = dynamic_cast<CBrick*>(coObjects->at(i));
			if (brick->GetType() == BRICK_TYPE_QUESTION_MUSHROOM_LEAF || brick->GetType() == BRICK_TYPE_QUESTION_GREEN_MUSHROOM)
			{
				continue;
			}
		}

		if (e->t > 0 && e->t <= 1.0f)
		{
			coEvents.push_back(e);
		}
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block 
		//x += min_tx * dx + nx * 0.5f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		//y += min_ty * dy + ny * 0.5f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		// Collision logic with the others
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CBrick::Render()
{
	int ani = -1;
	/*if (alive)
	{
		ani = BRICK_QUESTION_ANI_ALIVE;
	}
	else
		ani = BRICK_QUESTION_ANI_DEAD;*/

	// test
	if (type == BRICK_TYPE_QUESTION_NORMAL || type == BRICK_TYPE_QUESTION_MUSHROOM_LEAF)
	{
		if (alive)
		{
			ani = BRICK_QUESTION_ANI_ALIVE;
		}
		else
			ani = BRICK_QUESTION_ANI_DEAD;
	}
	else if (type == BRICK_TYPE_QUESTION_GREEN_MUSHROOM)
	{
		if (alive)
		{
			ani = BREAKABLE_BRICK_ANI_NORMAL;
		}
		else
			ani = BRICK_QUESTION_ANI_DEAD;
	}
	else if (type == BRICK_TYPE_BREAKABLE)
	{
		ani = BREAKABLE_BRICK_ANI_NORMAL;
	}
	else return;

		
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
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
	else // other brick
	{
		l = x;
		t = y;
		r = x + BRICK_BBOX_WIDTH;
		b = y + BRICK_BBOX_HEIGHT;
	}
}

void CBrick::SetState(int state)
{

}