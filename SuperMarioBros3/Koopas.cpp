#include "Koopas.h"

CKoopas::CKoopas(int koopas_type)
{
	type = koopas_type;
	SetState(KOOPAS_STATE_WALKING);
	nx = -1;
}
void CKoopas::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CBrick*>(coObjects->at(i)) && vy < 0)
		{
			continue;
		}

		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}
void CKoopas::FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy)
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
			ny = -0.01f;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}
void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPAS_STATE_DIE)
		return;

	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_SHELL || state == KOOPAS_STATE_SPINNING)
	{
		bottom = y + KOOPAS_BBOX_SHELL_HEIGHT;
	}
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	if (!holding)
	{
		vy += KOOPAS_GRAVITY * dt;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when koopas kicked 
	if (state != KOOPAS_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	//koopas shell is being held by mario
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (holding)
	{
		if (!mario->GetHolding())
		{
			holding = false;
			mario->StartKicking();
			mario->SetKicking(true);
			nx = mario->nx;
			SetState(KOOPAS_STATE_SPINNING);
		}
	}
	if (holding)
	{
		y = mario->y + 8;
		if (mario->nx > 0)
		{
			if (mario->GetLevel() == MARIO_LEVEL_BIG)
			{
				x = mario->x + MARIO_BIG_BBOX_WIDTH;
			}
			else if (mario->GetLevel() == MARIO_LEVEL_SMALL)
			{
				x = mario->x + MARIO_SMALL_BBOX_WIDTH;
				y = y - 10;
			}
			else if (mario->GetLevel() == MARIO_LEVEL_RACCOON)
			{
				x = mario->x + MARIO_RACCOON_BBOX_WIDTH;
			}
			else if (mario->GetLevel() == MARIO_LEVEL_FIRE)
			{
				x = mario->x + MARIO_FIRE_BBOX_WIDTH;
			}
		}
		else
		{
			if (mario->GetLevel() == MARIO_LEVEL_BIG)
			{
				x = mario->x - MARIO_BIG_BBOX_WIDTH;
			}
			else if (mario->GetLevel() == MARIO_LEVEL_SMALL)
			{
				x = mario->x - MARIO_SMALL_BBOX_WIDTH;
				y = y - 10;
			}
			else if (mario->GetLevel() == MARIO_LEVEL_RACCOON)
			{
				x = mario->x - MARIO_RACCOON_BBOX_WIDTH;
			}
			else
			{
				x = mario->x - MARIO_FIRE_BBOX_WIDTH;
			}
		}
		mario->SetCanHold(true);
	}

	if (state != KOOPAS_STATE_WALKING)
		CanPullBack = false;

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		if (CanPullBack && type == KOOPAS_TYPE_GREEN_WALK)
		{
			
		}
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

		// koopas shell is being held
		if(!holding)
			x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
			y += min_ty * dy + ny * 0.4f;

		if (ny != 0) vy = 0;

		if (ny < 0 && state == KOOPAS_STATE_SHELL)
		{
			vx = 0;
		}
		// Collision logic with the others koopas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CKoopas*>(e->obj)) // if e->obj is Koopas 
			{
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				if (e->nx != 0 && this->GetState() == KOOPAS_STATE_SPINNING)
				{
					if (koopas->GetState() != KOOPAS_STATE_SHELL)
					{
						koopas->SetState(KOOPAS_STATE_DIE);
					}
					else if (koopas->GetState() == KOOPAS_STATE_SPINNING || koopas->GetState() == KOOPAS_STATE_SHELL)
					{
						this->vx = -this->vx;
						koopas->vx = -koopas->vx;
					}
				}
			}

			else  // Collisions with other things  
			{
				if (nx != 0 && ny == 0)
				{
					if (dynamic_cast<CGoomba*>(e->obj))
					{
						CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
						if (goomba->GetState() != GOOMBA_STATE_DIE && (this->GetState() == KOOPAS_STATE_SPINNING || holding))
						{
							goomba->SetState(GOOMBA_STATE_DIE_BY_KICK);
						}
						else if (goomba->GetState() == GOOMBA_STATE_WALKING)
						{
							continue;
						}
					}
					else if (!dynamic_cast<CMario*>(e->obj))
					{
						vx = -vx;
					}
				}
			}
		}
	}

	if (vx < 0 && x < 0)
	{
		vx = -vx;
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CKoopas::Render()
{
	int ani = -1;
	switch (type)
	{
	case KOOPAS_TYPE_GREEN_WALK:
	{
		if (state == KOOPAS_STATE_DIE)
		{
			if (nx > 0)
			{
				ani = KOOPAS_GREEN_ANI_WALKING_RIGHT;
			}
			else
			{
				ani = KOOPAS_GREEN_ANI_WALKING_LEFT;
			}
		}
		else if (state == KOOPAS_STATE_SHELL)
		{
			if (ShellUp)
			{
				ani = KOOPAS_GREEN_ANI_SHELL_UP;
			}
			else
				ani = KOOPAS_GREEN_ANI_SHELL_DOWN;
		}
		else if (state == KOOPAS_STATE_SPINNING)
		{
			if (ShellUp)
			{
				if (vx > 0)	ani = KOOPAS_GREEN_ANI_SHELL_UP_SPIN_RIGHT;
				else ani = KOOPAS_GREEN_ANI_SHELL_UP_SPIN_LEFT;
			}
			else
			{
				if (vx > 0)	ani = KOOPAS_GREEN_ANI_SHELL_DOWN_SPIN_RIGHT;
				else ani = KOOPAS_GREEN_ANI_SHELL_DOWN_SPIN_LEFT;
			}
		}
		else if (vx > 0) ani = KOOPAS_GREEN_ANI_WALKING_RIGHT;
		else ani = KOOPAS_GREEN_ANI_WALKING_LEFT;
	}
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		vy = -KOOPAS_DIE_DEFLECT_SPEED;
		vx = -vx;
		break;
	case KOOPAS_STATE_WALKING:
		vx = -KOOPAS_WALKING_SPEED;
		break;
	case KOOPAS_STATE_SPINNING:
		if (nx > 0)
			vx = KOOPAS_WALKING_SPEED * 5;
		else
			vx = -KOOPAS_WALKING_SPEED * 5;
		break;
	case KOOPAS_STATE_SHELL:
		vx = 0;
		break;
	}

}