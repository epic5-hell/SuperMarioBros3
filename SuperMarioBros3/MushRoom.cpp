#include "MushRoom.h"

CMushRoom::CMushRoom(int mushroom_type)
{
	type = mushroom_type;
	SetState(MUSHROOM_STATE_IDLE);
}

void CMushRoom::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		/*else if(e->)*/
		else
			delete e;
	}
}

void CMushRoom::FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
		}

		if (c->t < min_ty && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
		}
		if (dynamic_cast<CMario*>(c->obj))
		{
			nx = ny = 0;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}



void CMushRoom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	// Simple fall down
	if (state == MUSHROOM_STATE_MOVING)
		vy += MUSHROOM_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (appear)
		CalcPotentialCollisions(coObjects, coEvents);

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);

		if (dynamic_cast<CBrick*>(obj))
		{
			CBrick* brick = dynamic_cast<CBrick*>(obj);
			if (brick->GetType() == BRICK_TYPE_QUESTION_GREEN_MUSHROOM)
			{
				if (!brick->GetIsAlive() && !brick->GetIsUsed())
				{
					if (!appear)
					{
						SetState(MUSHROOM_STATE_RISING);
						SetAppear(true);
						StartRising();
						brick->SetIsUsed(true);
					}
				}
			}
			else if (brick->GetType() == BRICK_TYPE_QUESTION_MUSHROOM_LEAF)
			{
				if (mario->GetLevel() == MARIO_LEVEL_SMALL)
				{
					if (!brick->GetIsAlive() && !brick->GetIsUsed())
					{
						if (!appear)
						{
							SetState(MUSHROOM_STATE_RISING);
							SetAppear(true);
							StartRising();
							brick->SetIsUsed(true);
						}
					}
				}
			}
		}
	}


	if (state == MUSHROOM_STATE_RISING)
	{
		if (GetTickCount64() - rising_start >= 300)
		{
			SetState(MUSHROOM_STATE_MOVING);
		}
	}

	// No collision occured, proceed normally
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

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		//if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (nx != 0 && ny == 0)
			{
				if (!dynamic_cast<CMario*>(e->obj))
				{
					vx = -vx;
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) 
		delete coEvents[i];
}

void CMushRoom::Render()
{
	int ani = -1;

	if (appear)
	{
		if (type == MUSHROOM_RED)
			ani = MUSHROOM_ANI_RED;
		else
			ani = MUSHROOM_ANI_GREEN;
	}
	else return;

	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CMushRoom::SetState(int state)
{
	CGameObject::SetState(state);
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (state)
	{
	case MUSHROOM_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case MUSHROOM_STATE_MOVING:
		if (mario->GetQBrickCollision() == true)
		{
			vx = -MUSHROOM_SPEED_MOVING;
		}
		else
		{
			vx = MUSHROOM_SPEED_MOVING;
		}
		break;
	case MUSHROOM_STATE_RISING:
		vy = MUSHROOM_SPEED_RISING;
		break;
	}
}
void CMushRoom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (appear)
	{
		left = x;
		top = y;
		right = left + MUSHROOM_BBOOX_WIDTH;
		bottom = top + MUSHROOM_BBOX_HEIGHT;
	}
	else
	{
		left = top = right = bottom = 0;
	}
}
