#include <algorithm>
#include <assert.h>
#include "Utils.h"
#include "Mario.h"
#include "Game.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Portal.h"
#include "Brick.h"
#include "MushRoom.h"
#include "Leaf.h"
#include "Alarm.h"

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
void CMario::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CBrick*>(coObjects->at(i)) && vy < 0)
		{
			CBrick* brick = dynamic_cast<CBrick*>(coObjects->at(i));
			if (brick->GetType() == BRICK_TYPE_BLOCK)
			{
				continue;
			}
		}

		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}
void CMario::FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult,
	float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy)
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
		if (dynamic_cast<CMushRoom*>(c->obj) || dynamic_cast<CLeaf*>(c->obj) || dynamic_cast<CFireBullet*>(c->obj) || dynamic_cast<CGoomba*>(c->obj))
		{
			ny = -0.0001f;
		}

		if (dynamic_cast<CBrick*>(c->obj))
		{
			CBrick* brick = dynamic_cast<CBrick*>(c->obj);
			if (brick->GetType() == BRICK_TYPE_BLOCK)
			{
				nx = 0;
			}
			else if (brick->GetType() == BRICK_TYPE_BREAKABLE)
			{
				if (!brick->GetShowBrick() && !brick->GetBreakBrick())
				{
					nx = ny = 0;
				}
			}
		}
		if (dynamic_cast<CKoopas*>(c->obj))
		{
			CKoopas* koopas = dynamic_cast<CKoopas*>(c->obj);
			if (c->ny > 0)
			{
				nx = ny = 0;
			}
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}
void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	if (GetTickCount64() - turning_start > MARIO_TURNING_TIME)
	{
		turning = false;
	}

	if (GetTickCount64() - kicking_start > MARIO_KICKING_TIME)
	{
		kicking = false;
	}

	if (GetTickCount64() - flying_start >= MARIO_FLYING_LIMIT_TIME)
	{
		canFly = false;
		flying = false;
		flying_start = 0;
	}

	if (!canFly)
	{
		canFall = true;
	}

	if (abs((y - checkY)) >= 1)
	{
		jumping = true;
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

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		checkY = y;

		if (ny != 0) vy = 0;

		if (ny < 0) // mario is on ground
		{
			jumping = false;
			falling = false;
			canFly = true;
			canFall = false;
		}

		if (ny < 0 && this->time_mario < MARIO_MAX_STACK) // mario power not enough
		{
			canFly = false;
		}

		//
		// Collision logic with other objects
		//


		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			// if Goomba
			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						if (goomba->GetType() != GOOMBA_TYPE_WINGS)
						{
							goomba->SetState(GOOMBA_STATE_DIE);
							vy = -MARIO_JUMP_DEFLECT_SPEED;
						}
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0 && !turning)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (level == MARIO_LEVEL_RACCOON)
							{
								level = MARIO_LEVEL_BIG;
								shooting = false;
								StartUntouchable();
							}
							else if (level == MARIO_LEVEL_BIG)
							{
								level = MARIO_LEVEL_SMALL;
								shooting = false;
								StartUntouchable();
							}
							else if (level == MARIO_LEVEL_FIRE)
							{
								level = MARIO_LEVEL_SMALL;
								shooting = false;
								StartUntouchable();
							}
							else // level = MARIO_LEVEL_SMALL
								SetState(MARIO_STATE_DIE);
						}
					}
					else if (level == MARIO_LEVEL_RACCOON && turning)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE_BY_KICK)
						{
							goomba->SetState(GOOMBA_STATE_DIE_BY_KICK);
						}
					}
				}
			}
			
			else if (dynamic_cast<CKoopas*>(e->obj))
			{
				CKoopas *koopas = dynamic_cast<CKoopas*>(e->obj);
				// jump on top >> Koopas turns into shell
				if (e->ny < 0)
				{
					if (koopas->GetType() == KOOPAS_TYPE_GREEN_WINGS)
					{
						koopas->SetType(KOOPAS_TYPE_GREEN_WALK);
						koopas->SetState(KOOPAS_STATE_WALKING);
						vy = -1.5f * MARIO_JUMP_DEFLECT_SPEED;
					}
					else // walking koopas
					{
						if (koopas->GetState() != KOOPAS_STATE_SHELL)
						{
							koopas->SetState(KOOPAS_STATE_SHELL);
							vy = -1.5f * MARIO_JUMP_DEFLECT_SPEED;
						}
						else if (koopas->GetState() == KOOPAS_STATE_SHELL)
						{
							vy = -1.5f * MARIO_JUMP_DEFLECT_SPEED;
							koopas->SetState(KOOPAS_STATE_SPINNING);
						}
					}
					delete koopas;
				}
				else if (e->nx != 0) //mario collisions with koopas
				{
					if (level == MARIO_LEVEL_RACCOON && turning)
					{
						koopas->SetState(KOOPAS_STATE_SHELL);
						koopas->SetShellUp(true);
						koopas->vy = -KOOPAS_SHELL_DEFLECT_SPEED;
						koopas->vx = 0.1f * (-nx);
					}
					else if (koopas->GetState() == KOOPAS_STATE_SHELL)
					{
						if (holding)
						{
							koopas->SetHolding(true);
						}
						else
						{
							StartKicking();
							kicking = true;
							koopas->nx = this->nx;
							koopas->SetState(KOOPAS_STATE_SPINNING);
						}
					}
					else if (untouchable == 0 && !kicking)
					{

						if (koopas->GetState() != KOOPAS_STATE_SHELL)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								if (level == MARIO_LEVEL_RACCOON)
								{
									level = MARIO_LEVEL_BIG;
									shooting = false;
									StartUntouchable();
								}
								else
								{
									level = MARIO_LEVEL_SMALL;
									shooting = false;
									StartUntouchable();
								}
							}
							else
							{
								SetState(MARIO_STATE_DIE);
							}

						}
					}
				}
				else if (e->ny > 0)
				{
					if (koopas->GetState() == KOOPAS_STATE_WALKING && untouchable == 0)
					{
						if (level > MARIO_LEVEL_SMALL)
						{
							if (level == MARIO_LEVEL_RACCOON)
							{
								level = MARIO_LEVEL_BIG;
								shooting = false;
								StartUntouchable();
							}
							else
							{
								level = MARIO_LEVEL_SMALL;
								shooting = false;
								StartUntouchable();
							}
						}
						else
						{
							SetState(MARIO_STATE_DIE);
						}
					}
					else if (koopas->GetState() == KOOPAS_STATE_SHELL)
					{
						StartKicking();
						kicking = true;
						koopas->nx = this->nx;
						koopas->SetState(KOOPAS_STATE_SPINNING);
					}
				}
			}
			else if (dynamic_cast<CBrick*> (e->obj))
			{
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);
				if (e->ny > 0)
				{
					if (brick->GetType() == BRICK_TYPE_QUESTION_NORMAL)
					{
						if (brick->GetAlive())
						{
							brick->SetAlive(false);

						}
					}
					else if (brick->GetType() == BRICK_TYPE_NEW || brick->GetType() == BRICK_TYPE_QUESTION_MUSHROOM_LEAF)
					{
						if (brick->GetAlive())
						{
							if (x > brick->x + BRICK_BBOX_WIDTH / 2)
							{
								SetQBrickCollision(true);
								brick->SetAlive(false);
							}
							else if (x <= brick->x + BRICK_BBOX_WIDTH / 2)
							{
								SetQBrickCollision(false);
								brick->SetAlive(false);
							}
							else
							{
								brick->SetAlive(false);
							}
						}
					}
					else if (brick->GetType() == BRICK_TYPE_BREAKABLE)
					{
						if (brick->GetShowBrick() == true)
						{
							return;
						}
					}

				}
				else if (e->nx != 0 || e->ny != 0)
				{
					if (brick->GetType() == BRICK_TYPE_BREAKABLE)
					{
						if (!brick->GetShowBrick() && !brick->GetBreakBrick())
						{
							brick->SetBreakBrick(true);
						}
					}
				}
			}
			else if (dynamic_cast<CMushRoom*>(e->obj))
			{
				CMushRoom* mushroom = dynamic_cast<CMushRoom*>(e->obj);
				if (mushroom->GetType() == MUSHROOM_RED)
				{
					if (level == MARIO_LEVEL_SMALL)
					{
						SetLevel(MARIO_LEVEL_BIG);
						mushroom->SetAppear(false);
					}
				}
				else // type = MUSHROOM_GREEN
				{
					mushroom->SetAppear(false);
				}
			}
			else if (dynamic_cast<CLeaf*>(e->obj))
			{
				CLeaf* leaf = dynamic_cast<CLeaf*>(e->obj);
				if (level == MARIO_LEVEL_SMALL)
				{
					SetLevel(MARIO_LEVEL_BIG);
					leaf->SetAppear(false);
				}
				else if (level == MARIO_LEVEL_BIG)
				{
					SetLevel(MARIO_LEVEL_RACCOON);
					leaf->SetAppear(false);
				}
				else if (level == MARIO_LEVEL_RACCOON)
				{
					leaf->SetAppear(false);
				}
			}
			else if (dynamic_cast<CAlarm*>(e->obj))
			{
				if (e->ny < 0)
				{
					CAlarm* alarm = dynamic_cast<CAlarm*>(e->obj);
					if (!alarm->Getactivate())
					{
						for (UINT i = 0; i < coObjects->size(); i++)
						{
							LPGAMEOBJECT obj = coObjects->at(i);
							if (dynamic_cast<CBrick*>(obj))
							{
								CBrick* brick = dynamic_cast<CBrick*>(obj);
								if (brick->GetType() == BRICK_TYPE_BREAKABLE)
								{
									if (brick->GetShowBrick())
									{
										brick->SetShowBrick(false);
										brick->SetRevive(true);
									}
									else if (!brick->GetShowBrick())
									{

									}
								}
							}
						}
						alarm->Setactivate(true);
						vy = -1.5f * MARIO_JUMP_DEFLECT_SPEED;
					}

				}
			}
			/*else if (dynamic_cast<CPortal *>(e->obj))
			{
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}*/
		}
	}

	if (vx < 0 && x < 0)  // right edge
	{
		x = 0;
	}

	//if (vy < 0 && y < 0) y = 0; //max height
		// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}

void CMario::Render()
{
	int ani = -1;

	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;

	// opportunity: fall, fly, turning tail, hold, fire, jump

	// state = FALL
	else if (falling)
	{
		if (level == MARIO_LEVEL_RACCOON)
		{
			if (nx > 0) ani = MARIO_ANI_RACCOON_FALLING_LEFT;
			else ani = MARIO_ANI_RACCOON_FALLING_RIGHT;
		}
		else return;
	}
	// state = FLY
	else if (flying)
	{
		if (nx > 0) ani = MARIO_ANI_RACCOON_FLYING_RIGHT;
		else ani = MARIO_ANI_RACCOON_FLYING_LEFT;
	}
	// state = TURN
	else if (turning)
	{
		if (nx < 0)
		{
			ani = MARIO_ANI_RACCOON_TURNING_LEFT;
		}
		else ani = MARIO_ANI_RACCOON_TURNING_RIGHT;
	}

	// state = HOLD
	else if (canHold)
	{
		if (state == MARIO_STATE_IDLE)
		{
			if (level == MARIO_LEVEL_BIG)
			{
				if (nx > 0) ani = MARIO_ANI_BIG_HOLDING_IDLE_RIGHT;
				else ani = MARIO_ANI_BIG_HOLDING_IDLE_LEFT;
			}
			else if (level == MARIO_LEVEL_SMALL)
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_HOLDING_IDLE_RIGHT;
				else ani = MARIO_ANI_SMALL_HOLDING_IDLE_LEFT;
			}
			else if (level == MARIO_LEVEL_RACCOON)
			{
				if (nx > 0) ani = MARIO_ANI_RACCOON_HOLDING_IDLE_RIGHT;
				else ani = MARIO_ANI_RACCOON_HOLDING_IDLE_LEFT;
			}
			else if (level == MARIO_LEVEL_FIRE)
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_HOLDING_IDLE_RIGHT;
				else ani = MARIO_ANI_FIRE_HOLDING_IDLE_LEFT;
			}
		}
		else
		{
			if (level == MARIO_LEVEL_BIG)
			{
				if (nx > 0) ani = MARIO_ANI_BIG_HOLDING_WALK_RIGHT;
				else ani = MARIO_ANI_BIG_HOLDING_WALK_LEFT;
			}
			else if (level == MARIO_LEVEL_SMALL)
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_HOLDING_WALK_RIGHT;
				else ani = MARIO_ANI_SMALL_HOLDING_WALK_LEFT;
			}
			else if (level == MARIO_LEVEL_RACCOON)
			{
				if (nx > 0) ani = MARIO_ANI_RACCOON_HOLDING_WALK_RIGHT;
				else ani = MARIO_ANI_RACCOON_HOLDING_WALK_LEFT;
			}
			else if (level == MARIO_LEVEL_FIRE)
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_HOLDING_WALK_RIGHT;
				else ani = MARIO_ANI_FIRE_HOLDING_WALK_LEFT;
			}
		}
	}

	// state = SHOOT
	else if (shooting && level == MARIO_LEVEL_FIRE)
	{
		if (nx > 0) ani = MARIO_ANI_FIRE_SHOOTING_RIGHT;
		else ani = MARIO_ANI_FIRE_SHOOTING_LEFT;
	}
	// state = JUMP
	else if (jumping)
	{
		if (level == MARIO_LEVEL_BIG)
		{
			if (vy < 0)
			{
				if (nx > 0) ani = MARIO_ANI_BIG_JUMPING_RIGHT;
				else ani = MARIO_ANI_BIG_JUMPING_LEFT;
			}
			else
			{
				if (nx > 0) ani = MARIO_ANI_BIG_FALLING_RIGHT;
				else ani = MARIO_ANI_BIG_FALLING_LEFT;
			}
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			if (nx > 0) ani = MARIO_ANI_SMALL_JUMPING_RIGHT;
			else ani = MARIO_ANI_SMALL_JUMPING_LEFT;
		}
		else if (level == MARIO_LEVEL_RACCOON)
		{
			if (vy < 0)
			{
				if (nx > 0) ani = MARIO_ANI_RACCOON_JUMPING_RIGHT;
				else ani = MARIO_ANI_RACCOON_JUMPING_LEFT;
			}
			else
			{
				if (nx < 0) ani = MARIO_ANI_RACCOON_FALLING_RIGHT;
				else ani = MARIO_ANI_RACCOON_FALLING_LEFT;
			}
		}
		else if (level == MARIO_LEVEL_FIRE)
		{
			if (vy < 0)
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_JUMPING_RIGHT;
				else ani = MARIO_ANI_FIRE_JUMPING_LEFT;
			}
			else
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_FALLING_RIGHT;
				else ani = MARIO_ANI_FIRE_FALLING_LEFT;
			}
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
	// state = SIT DOWN
	else if (state == MARIO_STATE_SITDOWN)
	{
		if (level == MARIO_LEVEL_BIG)
		{
			if (nx > 0) ani = MARIO_ANI_BIG_SITDOWN_RIGHT;
			else ani = MARIO_ANI_BIG_SITDOWN_LEFT;
		}
		if (level == MARIO_LEVEL_SMALL)
		{
			if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
			else ani = MARIO_ANI_SMALL_IDLE_LEFT;
		}
		else if (level == MARIO_LEVEL_RACCOON)
		{
			if (nx > 0) ani = MARIO_ANI_RACCOON_SITDOWN_RIGHT;
			else ani = MARIO_ANI_RACCOON_SITDOWN_LEFT;
		}
		else if (level == MARIO_LEVEL_FIRE)
		{
			if (nx > 0) ani = MARIO_ANI_FIRE_SITDOWN_RIGHT;
			else ani = MARIO_ANI_FIRE_SITDOWN_LEFT;
		}
	}

	//	 state = RUN
	else if (state == MARIO_STATE_RUNNING_RIGHT) //running right
	{
		if (vx >= MARIO_RUNNING_SPEED * 4) // MAX SPEED
		{
			if (level == MARIO_LEVEL_SMALL)
			{
				ani = MARIO_ANI_SMALL_MAX_SPEED_RIGHT;
			}
			else if (level == MARIO_LEVEL_BIG)
			{
				ani = MARIO_ANI_BIG_MAX_SPEED_RIGHT;
			}
			else if (level == MARIO_LEVEL_RACCOON)
			{
				ani = MARIO_ANI_RACCOON_MAX_SPEED_RIGHT;
			}
			else if (level == MARIO_LEVEL_FIRE)
			{
				ani = MARIO_ANI_FIRE_MAX_SPEED_RIGHT;
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
				ani = MARIO_ANI_SMALL_MAX_SPEED_LEFT;
			}
			else if (level == MARIO_LEVEL_BIG)
			{
				ani = MARIO_ANI_BIG_MAX_SPEED_LEFT;
			}
			else if (level == MARIO_LEVEL_RACCOON)
			{
				ani = MARIO_ANI_RACCOON_MAX_SPEED_LEFT;
			}
			else if (level == MARIO_LEVEL_FIRE)
			{
				ani = MARIO_ANI_FIRE_MAX_SPEED_LEFT;
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

	// state = KICK
	else if (kicking)
	{
		if (level == MARIO_LEVEL_BIG)
		{
			if (nx > 0) ani = MARIO_ANI_BIG_KICKING_RIGHT;
			else ani = MARIO_ANI_BIG_KICKING_LEFT;
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			if (nx > 0) ani = MARIO_ANI_SMALL_KICKING_RIGHT;
			else ani = MARIO_ANI_SMALL_KICKING_LEFT;
		}
		else if (level == MARIO_LEVEL_RACCOON)
		{
			if (nx > 0) ani = MARIO_ANI_RACCOON_KICKING_RIGHT;
			else ani = MARIO_ANI_RACCOON_KICKING_LEFT;
		}
		else if (level == MARIO_LEVEL_FIRE)
		{
			if (nx > 0) ani = MARIO_ANI_FIRE_KICKING_RIGHT;
			else ani = MARIO_ANI_FIRE_KICKING_LEFT;
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

	RenderBoundingBox();
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		nx = 1;
		if (BrakeCalc() == false)
		{
			vx = MARIO_WALKING_SPEED / 2;
		}
		break;
	case MARIO_STATE_WALKING_LEFT:
		nx = -1;
		if (BrakeCalc() == false)
		{
			vx = -MARIO_WALKING_SPEED / 2;
		}
		break;
	case MARIO_STATE_RUNNING_RIGHT:
		nx = 1;
		if (BrakeCalc() == false)
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
		if (BrakeCalc() == false)
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
	case MARIO_STATE_SITDOWN:
		vx = 0;
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
	case MARIO_STATE_TURNING_TAIL:
		vx = 0;
		break;
	case MARIO_STATE_FLYING_RIGHT:
		vx = MARIO_WALKING_SPEED;
		vy = -MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case MARIO_STATE_FLYING_LEFT:
		vx = -MARIO_WALKING_SPEED;
		vy = -MARIO_WALKING_SPEED;
		nx = -1;
		break;
	case MARIO_STATE_FALLING_DOWN:
		vy = 0.08f;
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		vx = 0;
		break;
	}
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == MARIO_STATE_DIE)
	{
		left = top = right = bottom = 0;
		return;
	}
	left = x;
	top = y;

	if (state == MARIO_STATE_SITDOWN && level != MARIO_LEVEL_SMALL)
	{
		top += 9;
	}
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

