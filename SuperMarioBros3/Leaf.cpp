#include "Leaf.h"

CLeaf::CLeaf()
{
	SetState(LEAF_STATE_IDLE);
}

void CLeaf::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (!dynamic_cast<CMario*>(coObjects->at(i)))
		{
			continue;
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

void CLeaf::FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy)
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

void CLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (appear)
	{
		left = x;
		top = y;
		right = left + LEAF_BBOX_WIDTH;
		bottom = top + LEAF_BBOX_HEIGHT;
	}
	else
	{
		left = top = right = bottom = 0;
	}
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);


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
			if (brick->GetType() == BRICK_TYPE_QUESTION_MUSHROOM_LEAF)
			{
				if (!brick->GetAlive() && !brick->GetUsed())
				{
					if (mario->GetLevel() != MARIO_LEVEL_SMALL)
					{
						if (!appear)
						{
							SetState(LEAF_STATE_RISING);
							SetAppear(true);
							StartRising();
							brick->SetUsed(true);
						}
					}
				}
			}
		}
	}

	if (state == LEAF_STATE_RISING)
	{
		if (GetTickCount64() - rising_start >= 1500)
		{
			SetState(LEAF_STATE_SETTING);
		}
	}

	if (state == LEAF_STATE_SETTING)
	{
		if (setting_start == 0)
		{
			StartSetting();
		}
		else if (GetTickCount64() - setting_start >= 700)
		{
			vx = -vx;
			setting_start = 0;
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

		// Collision logic with the others Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
		}
	}


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) 
		delete coEvents[i];
}


void CLeaf::Render()
{
	int ani = -1;

	if (appear)
	{
		if (vx > 0)
			ani = LEAF_ANI_RIGHT;
		else // vx < 0
			ani = LEAF_ANI_LEFT;
	}
	else return;

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CLeaf::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case LEAF_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case LEAF_STATE_RISING:
		vy = -0.04f;
		break;
	case LEAF_STATE_SETTING:
		vx = 0.04f;
		vy = 0.04f;
		break;
	}
}






