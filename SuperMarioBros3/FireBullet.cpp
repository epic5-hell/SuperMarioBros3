#include "FireBullet.h"


CFireBullet::CFireBullet() : CGameObject()
{
	IsUsed = false;
	SetState(BULLET_STATE_DISAPPEAR);
	height = 0;
}

void CFireBullet::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (dynamic_cast<CFireBullet*>(e->obj))
			continue;

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}
	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CFireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	/*vy += BULLET_GRAVITY * dt;*/

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	if(IsUsed)
		CalcPotentialCollisions(coObjects, coEvents);
	
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (mario->GetShooting() && !IsUsed)
	{
		if (!mario->GetShooted())
		{
			y = mario->y;
			if (mario->nx > 0)
			{
				x = mario->x + MARIO_FIRE_BBOX_WIDTH + 1;
				vx = BULLET_FLYING_SPEED / 2;
			}
			else
			{
				x = mario->x - MARIO_FIRE_BBOX_WIDTH - 1;
				vx = -BULLET_FLYING_SPEED / 2;
			}
			SetState(BULLET_STATE_FLYING);
			vy = 0.1f;
			height = mario->y;
			mario->SetShooted(true);
		}
	}

	if (!IsUsed)
	{
		SetState(BULLET_STATE_DISAPPEAR);
	}

	if (this->y <= height)
		vy = 0.1f;


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

		//x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		/*if (ny > 0) vy = 0.2f;
		else if (ny < 0) vy = -0.2f;*/

		if (ny > 0)
			height = this->y;

		if (ny != 0) vy = -vy;

		//
		// Collision logic with other objects

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CKoopas*>(e->obj)) // if e->obj is Koopas
			{
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				if (e->nx != 0 || e->ny < 0)
				{
					if (koopas->GetState() != KOOPAS_STATE_SHELL)
					{
						koopas->SetState(KOOPAS_STATE_SHELL);
						IsUsed = false;
						//SetState(BULLET_STATE_DISAPPEAR);
					}
					else
					{
						koopas->SetState(KOOPAS_STATE_DIE);
						IsUsed = false;
						//SetState(BULLET_STATE_DISAPPEAR);
					}
				}

			}
			else if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				if (e->nx != 0 || e->ny < 0)
				{
					goomba->SetState(GOOMBA_STATE_DIE_BY_KICK);
					IsUsed = false;
					//SetState(BULLET_STATE_DISAPPEAR);
				}
			}
			else if (dynamic_cast<CMario*>(e->obj))
			{
				IsUsed = false;
			}
			else if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);
				if (brick->getType() == BRICK_TYPE_NORMAL)
				{
					if (e->ny > 0)
					{
						this->vy = -this->vy;
					}
					else if (nx != 0 && ny == 0)
						IsUsed = false;
				}
				else if (brick->getType() == BRICK_TYPE_BLOCK)
				{
					if (e->ny > 0)
					{
						this->vy = -this->vy;
					}
				}
				else // Collisions with other things  
				{
					if (nx != 0 && ny == 0)
						IsUsed = false;
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void CFireBullet::Render()
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	int ani = -1;
	if (IsUsed)
	{
		if (mario->nx > 0)
		{
			ani = BULLET_ANI_FLYING_RIGHT;
		}
		else
			ani = BULLET_ANI_FLYING_LEFT;
	}

	else return;


	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void CFireBullet::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BULLET_BBOX_WIDTH;
	b = y + BULLET_BBOX_HEIGHT;
}
void  CFireBullet::SetState(int state)
{
	CGameObject::SetState(state);
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (state)
	{
	case BULLET_STATE_FLYING:
		if (nx > 0)
			vx = BULLET_FLYING_SPEED;
		else
			vx = -BULLET_FLYING_SPEED;
		IsUsed = true;
		break;
	case BULLET_STATE_DISAPPEAR:
		vx = 0;
		vy = 0;
		//IsUsed = false;
		SetPosition(1000, 1000);
		break;
	}


}