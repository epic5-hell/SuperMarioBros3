#include "Alarm.h"

CAlarm::CAlarm()
{
}

void CAlarm::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
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

void CAlarm::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (appear)
	{
		if (activate)
		{
			left = top = right = bottom = 0;
		}
		else
		{
			left = x;
			top = y;
			right = left + ALARM_BBOX_WIDTH;
			bottom = top + ALARM_BBOX_HEIGHT;
		}
	}
	else
	{
		left = top = right = bottom = 0;
	}
}

void CAlarm::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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
			if (brick->GetType() == BRICK_TYPE_NEW)
			{
				if (!brick->GetAlive() && !brick->GetUsed())
				{
					if (!appear)
					{
						SetAppear(true);
						brick->SetUsed(true);
						DebugOut(L"mario touch alarm \n");
					}
				}
			}
		}
	}

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
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
		//x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		//y += min_ty * dy +  ny * 0.4f;

		//if (nx != 0) vx = 0;
		//if (ny != 0) vy = 0;

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

void CAlarm::Render()
{
	int ani = -1;

	if (appear && !activate)
	{
		ani = ALARM_ANI_SHOW;
	}
	else if (appear && activate)
	{
		//return;
		ani = ALARM_ANI_ACTIVE;
	}
	else return;

	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void CAlarm::SetState(int state)
{
}
