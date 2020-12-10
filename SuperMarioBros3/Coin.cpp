#include "Coin.h"

CCoin::CCoin(int coin_type)
{
	type = coin_type;
	if (type == BIG_COIN)
	{
		appear = true;
	}
	else // type = SMALL COIN
	{
		appear = false;
		coin_up = false;
		coin_down = false;
		vy = 0;
	}
}

void CCoin::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
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

void CCoin::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!appear || type == SMALL_COIN)
		return;
	else
	{
		left = x;
		top = y;
		right = x + BIG_COIN_BBOX_WIDTH;
		bottom = y + BIG_COIN_BBOX_HEIGHT;
	}
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (appear && type != SMALL_COIN)
		CalcPotentialCollisions(coObjects, coEvents);

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CBrick*>(obj))
		{
			CBrick* brick = dynamic_cast<CBrick*>(obj);
			if (brick->GetType() == BRICK_TYPE_QUESTION_NORMAL)
			{
				if (!brick->GetAlive() && !brick->GetUsed())
				{
					if (!appear && this->type == SMALL_COIN)
					{
						if ((this->x == brick->x + 4) && (this->y == brick->y - 15))
						{
							appear = true;
							vy = -SMALL_COIN_DEFLECT_SPEED;
							brick->SetUsed(true);
							SetCoinUp(true);
							StartUpDown();
						}
					}

				}
			}
			
		}
	}



	if (coin_up == true)
	{
		if (GetTickCount64() - up_down >= 250)
		{
			SetCoinUp(false);
			SetCoinDown(true);
			StartUpDown();
			vy = SMALL_COIN_DEFLECT_SPEED;
		}
	}

	if (coin_down == true)
	{
		if (GetTickCount64() - up_down >= 250)
		{
			appear = false;
			SetCoinDown(false);
			vy = 0;
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
		//x += min_tx * dx + nx * 0.5f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		//y += min_ty * dy + ny * 0.5f;

		/*if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;*/

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

void CCoin::Render()
{
	int ani = -1;

	if (appear)
	{
		if (type == BIG_COIN)
		{
			ani = BIG_COIN_ANI;
		}
		else if (type == SMALL_COIN)
		{
			ani = SMALL_COIN_ANI;
		}
	}
	else return;
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void CCoin::SetState(int state)
{
}
