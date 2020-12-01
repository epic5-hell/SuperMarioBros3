#include "Goomba.h"

CGoomba::CGoomba(int goomba_type)
{
	type = goomba_type;
	SetState(GOOMBA_STATE_WALKING);
}
void CGoomba::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}
void CGoomba::FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy)
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
			ny = -0.001f;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	if (state == GOOMBA_STATE_DIE)
	{
		top = y + 7;
		right = left + GOOMBA_NORMAL_BBOX_WIDTH;
		bottom = y + GOOMBA_NORMAL_BBOX_HEIGHT;
	}
	else if (state == GOOMBA_STATE_WALKING)
	{
		if (type == GOOMBA_TYPE_NORMAL)
		{
			right = x + GOOMBA_NORMAL_BBOX_WIDTH;
			bottom = y + GOOMBA_NORMAL_BBOX_HEIGHT;
		}
		else
		{
			right = x + GOOMBA_WINGS_BBOX_WIDTH;
			bottom = y + GOOMBA_WINGS_BBOX_HEIGHT;
		}
	}
	else if (state == GOOMBA_STATE_LOSE_WINGS)
	{
		top = y + 7;
		right = x + GOOMBA_LOSE_WINGS_BBOX_WIDTH;
		bottom = y + GOOMBA_NORMAL_BBOX_HEIGHT;
	}
	else
	{
		left = top = right = bottom = 0;
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	// simple fall down
	vy += GOOMBA_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when goomba died
	if (state != GOOMBA_STATE_DIE_BY_KICK || state != GOOMBA_STATE_DISAPPEAR)
		CalcPotentialCollisions(coObjects, coEvents);

	if (state == GOOMBA_STATE_DIE)
	{
		if (dying_start == 0)
		{
			StartDying();
		}
		if (GetTickCount64() - dying_start >= 200)
		{
			SetState(GOOMBA_STATE_DISAPPEAR);
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
		float min_tx, min_ty;
		float nx = 0;
		float ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// groomba walks on block 
		if (state != GOOMBA_STATE_DIE)
		{
			x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
			y += min_ty * dy + ny * 0.4f;
		}

		if (ny != 0) vy = 0;

		// Collision with other goomba
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				if (e->nx != 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->vx = -goomba->vx;
						this->vx = -this->vx;
					}
				}
			}
			else // Collisions with the others
			{
				if (nx != 0 && ny == 0 && !dynamic_cast<CKoopas*>(e->obj))
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

void CGoomba::Render()
{
	int ani = -1;
	switch (type)
	{
	case GOOMBA_TYPE_NORMAL:
	{
		ani = GOOMBA_NORMAL_ANI_WALKING;
		if (state == GOOMBA_STATE_DISAPPEAR)
			return;
		else if (state == GOOMBA_STATE_DIE)
		{
			ani = GOOMBA_NORMAL_ANI_DIE;
		}
		else if (state == GOOMBA_STATE_DIE_BY_KICK)
		{
			ani = GOOMBA_NORMAL_ANI_WALKING;
		}
		break;
	}
	case GOOMBA_TYPE_WINGS:
		return;
		break;
	}
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	case GOOMBA_STATE_DIE_BY_KICK:
		vy = -GOOMBA_DIE_DEFLECT_SPEED;
		vx = -vx;
		break;
	case GOOMBA_STATE_LOSE_WINGS:
		vy = 0;
		break;
	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
		break;
	}
}
