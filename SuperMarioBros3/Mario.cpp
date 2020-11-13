#include <algorithm>
#include <assert.h>
#include "Utils.h"
#include "Mario.h"
#include "Game.h"
#include "Goomba.h"
#include "Portal.h"
#include "Brick.h"

CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_SMALL;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);

	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y;

}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += MARIO_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		//IsTouchingGround = false;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx*dx + nx*0.4f;
		y += min_ty*dy + ny*0.4f;

		if (ny != 0) vy = 0;

		if (ny < 0) // mario is jumping
		{
			jumping = false;
		}

		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			// if Goomba
			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else 
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			} // if Portal
			else if (dynamic_cast<CPortal *>(e->obj))
			{
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	if (vx < 0 && x < 0) x = 0; // right edge
	if (vy < 0 && y < 0) y = 0; //max height
}

void CMario::Render()
{
	int ani = -1;

	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;

	// opportunity: fly, fall, turning tail, hold, fire, jump

	// state = JUMP
	else if (jumping == true)
	{
		if (level == MARIO_LEVEL_BIG)
		{
			if (nx > 0) ani = MARIO_ANI_BIG_JUMPING_RIGHT;
			else ani = MARIO_ANI_BIG_JUMPING_LEFT;
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			if (nx > 0) ani = MARIO_ANI_SMALL_JUMPING_RIGHT;
			else ani = MARIO_ANI_SMALL_JUMPING_LEFT;
		}
		else if (level == MARIO_LEVEL_RACCOON)
		{
			if (nx > 0) ani = MARIO_ANI_RACCOON_JUMPING_RIGHT;
			else ani = MARIO_ANI_RACCOON_JUMPING_LEFT;
		}
		else if (level == MARIO_LEVEL_FIRE)
		{
			if (nx > 0) ani = MARIO_ANI_FIRE_JUMPING_RIGHT;
			else ani = MARIO_ANI_FIRE_JUMPING_LEFT;
		}
	}

	// state = IDLE
	else if (state == MARIO_STATE_IDLE)
	{
		if (level == MARIO_LEVEL_BIG)
		{
			if (nx > 0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
			else ani = MARIO_ANI_BIG_IDLE_LEFT;
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
			else ani = MARIO_ANI_SMALL_IDLE_LEFT;
		}
		else if (level == MARIO_LEVEL_RACCOON)
		{
			if (nx > 0) ani = MARIO_ANI_RACCOON_IDLE_RIGHT;
			else ani = MARIO_ANI_RACCOON_IDLE_LEFT;
		}
		else if (level == MARIO_LEVEL_FIRE)
		{
			if (nx > 0)ani = MARIO_ANI_FIRE_IDLE_RIGHT;
			else ani = MARIO_ANI_FIRE_IDLE_LEFT;
		}
	}

	// state = BRAKE
	else if (brake)
	{
		if (level == MARIO_LEVEL_BIG)
		{
			if (nx > 0) ani = MARIO_ANI_BIG_BRAKING_RIGHT;
			else ani = MARIO_ANI_BIG_BRAKING_LEFT;
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			if (nx > 0) ani = MARIO_ANI_SMALL_BRAKING_RIGHT;
			else ani = MARIO_ANI_SMALL_BRAKING_LEFT;
		}
		else if (level == MARIO_LEVEL_RACCOON)
		{
			if (nx > 0) ani = MARIO_ANI_RACCOON_BRAKING_RIGHT;
			else ani = MARIO_ANI_RACCOON_BRAKING_LEFT;
		}
		else if (level == MARIO_LEVEL_FIRE)
		{
			if (nx > 0) ani = MARIO_ANI_FIRE_BRAKING_RIGHT;
			else ani = MARIO_ANI_FIRE_BRAKING_LEFT;
		}
	}

	//	 state = RUN
	else if (state == MARIO_STATE_RUNNING_RIGHT) //running right
	{
		if (vx >= MARIO_RUNNING_SPEED * 4) // MAX SPEED
		{
			if (level == MARIO_LEVEL_SMALL)
			{
				ani = MARIO_ANI_SMALL_RUNNING_MAX_SPEED_RIGHT;
			}
			else if (level == MARIO_LEVEL_BIG)
			{
				ani = MARIO_ANI_BIG_RUNNING_MAX_SPEED_RIGHT;
			}
			else if (level == MARIO_LEVEL_RACCOON)
			{
				ani = MARIO_ANI_RACCOON_RUNNING_MAX_SPEED_RIGHT;
			}
			else if (level == MARIO_LEVEL_FIRE)
			{
				ani = MARIO_ANI_FIRE_RUNNING_MAX_SPEED_RIGHT;
			}
		}
		else // NORMALLY
		{
			if (level == MARIO_LEVEL_SMALL)
			{
				ani = MARIO_ANI_SMALL_RUNNING_RIGHT;
			}
			else if (level == MARIO_LEVEL_BIG)
			{
				ani = MARIO_ANI_BIG_RUNNING_RIGHT;
			}
			else if (level == MARIO_LEVEL_RACCOON)
			{
				ani = MARIO_ANI_RACCOON_RUNNING_RIGHT;
			}
			else if (level == MARIO_LEVEL_FIRE)
			{
				ani = MARIO_ANI_FIRE_RUNNING_RIGHT;
			}
		}
	}
	else if (state == MARIO_STATE_RUNNING_LEFT) // running left
	{
		if (vx == -MARIO_RUNNING_SPEED * 4) // MAX SPEED
		{
			if (level == MARIO_LEVEL_SMALL)
			{
				ani = MARIO_ANI_SMALL_RUNNING_MAX_SPEED_LEFT;
			}
			else if (level == MARIO_LEVEL_BIG)
			{
				ani = MARIO_ANI_BIG_RUNNING_MAX_SPEED_LEFT;
			}
			else if (level == MARIO_LEVEL_RACCOON)
			{
				ani = MARIO_ANI_RACCOON_RUNNING_MAX_SPEED_LEFT;
			}
			else if (level == MARIO_LEVEL_FIRE)
			{
				ani = MARIO_ANI_FIRE_RUNNING_MAX_SPEED_LEFT;
			}
		}
		else // NORMALLY
		{
			if (level == MARIO_LEVEL_SMALL)
			{
				ani = MARIO_ANI_SMALL_RUNNING_LEFT;
			}
			else if (level == MARIO_LEVEL_BIG)
			{
				ani = MARIO_ANI_BIG_RUNNING_LEFT;
			}
			else if (level == MARIO_LEVEL_RACCOON)
			{
				ani = MARIO_ANI_RACCOON_RUNNING_LEFT;
			}
			else if (level == MARIO_LEVEL_FIRE)
			{
				ani = MARIO_ANI_FIRE_RUNNING_LEFT;
			}
		}
	}

	// state = WALK
	else if (nx > 0) // mario is walking right
	{
		if (level == MARIO_LEVEL_BIG)
		{
			ani = MARIO_ANI_BIG_WALKING_RIGHT;
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			ani = MARIO_ANI_SMALL_WALKING_RIGHT;
		}
		else if (level == MARIO_LEVEL_RACCOON)
		{
			ani = MARIO_ANI_RACCOON_WALKING_RIGHT;
		}
		else if (level == MARIO_LEVEL_FIRE)
		{
			ani = MARIO_ANI_FIRE_WALKING_RIGHT;
		}
	}
	else if (nx < 0) // mario is walking left
	{
		if (level == MARIO_LEVEL_BIG)
		{
			ani = MARIO_ANI_BIG_WALKING_LEFT;
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			ani = MARIO_ANI_SMALL_WALKING_LEFT;
		}
		else if (level == MARIO_LEVEL_RACCOON)
		{
			ani = MARIO_ANI_RACCOON_WALKING_LEFT;
		}
		else if (level == MARIO_LEVEL_FIRE)
		{
			ani = MARIO_ANI_FIRE_WALKING_LEFT;
		}
	}




	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y, alpha);

	//RenderBoundingBox();
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		nx = 1;
		if (BrakeCalculation() == false)
		{
			vx = MARIO_WALKING_SPEED / 2;
		}
		break;
	case MARIO_STATE_WALKING_LEFT:
		nx = -1;
		if (BrakeCalculation() == false)
		{
			vx = -MARIO_WALKING_SPEED / 2;
		}
		break;
	case MARIO_STATE_RUNNING_RIGHT:
		nx = 1;
		if (BrakeCalculation() == false)
		{
			if (vx >= MARIO_RUNNING_SPEED * 4)
			{
				vx = MARIO_RUNNING_SPEED * 4;
			}
			else
			{
				vx = MARIO_ACCELERATION * 4 * time_mario;
			}
		}
		break;
	case MARIO_STATE_RUNNING_LEFT:
		nx = -1;
		if (BrakeCalculation() == false)
		{
			if (vx <= -MARIO_RUNNING_SPEED * 4)
			{
				vx = -MARIO_RUNNING_SPEED * 4;
			}
			else
			{
				vx = -(MARIO_ACCELERATION * 4 * time_mario);
			}
		}
		break;
	case MARIO_STATE_JUMP:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		vy = -MARIO_JUMP_SPEED_Y;
		break;
	case MARIO_STATE_IDLE: 
		vx = 0;
		break;
	case MARIO_STATE_SPEED_DOWN:
		if (vx > 0)
		{
			vx -= MARIO_SPEED_DOWN;
		}
		else if (vx < 0)
		{
			vx += MARIO_SPEED_DOWN;
		}
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	}
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y; 

	if (level == MARIO_LEVEL_BIG)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_RACCOON)
	{
		right = x + MARIO_RACCOON_BBOX_WIDTH;
		bottom = y + MARIO_RACCOON_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_FIRE)
	{
		right = x + MARIO_FIRE_BBOX_WIDTH;
		bottom = y + MARIO_FIRE_BBOX_HEIGHT;
	}
}



/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

