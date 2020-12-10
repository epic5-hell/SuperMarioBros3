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
			if (brick->GetType() == BRICK_TYPE_QUESTION_MUSHROOM_LEAF || brick->GetType() == BRICK_TYPE_NEW)
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

void CBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (type == BRICK_TYPE_BLOCK)
	{
		left = x;
		top = y;
		right = left + BRICK_BBOX_WIDTH;
		bottom = top;
	}
	else if (type == BRICK_TYPE_BREAKABLE)
	{
		if (show_brick && !break_brick)
		{
			left = x;
			top = y;
			right = left + BREAKABLE_BRICK_BBOX_WIDTH;
			bottom = top + BREAKABLE_BRICK_BBOX_HEIGHT;
		}
		else if (!show_brick && !break_brick)
		{
			left = x;
			top = y;
			right = left + BRICK_BIG_COIN_BBOX_WIDTH;
			bottom = top + BRICK_BIG_COIN_BBOX_HEIGHT;
		}
		else
		{
			left = top = right = bottom = 0;
		}
	}
	else // other brick
	{
		left = x;
		top = y;
		right = left + BRICK_BBOX_WIDTH;
		bottom = top + BRICK_BBOX_HEIGHT;
	}
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

	if (revive && reviving_start == 0)
	{
		StartRevive();
	}

	if (!GetShowBrick())
	{
		if (GetTickCount64() - reviving_start >= TIME_BREAKABLE_BRICK_REVIVE)
		{
			SetShowBrick(true);
		}
	}
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

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
	else if (type == BRICK_TYPE_NEW)
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
		if (!break_brick)
		{
			if (show_brick)
			{
				ani = BREAKABLE_BRICK_ANI_NORMAL;
			}
			else
			{
				ani = BREAKABLE_BRICK_ANI_COIN;
			}
		}
		else return;
	}
	else return;

		
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CBrick::SetState(int state)
{

}