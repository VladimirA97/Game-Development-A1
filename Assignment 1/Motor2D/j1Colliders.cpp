#include "j1Colliders.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"

j1Colliders::j1Colliders()
{
	name.create("Colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		colliders[i] = nullptr;
	}

	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WALL][COLLIDER_PREVENT] = true;
	matrix[COLLIDER_WALL][COLLIDER_NEXT_MAP] = false;
	matrix[COLLIDER_WALL][COLLIDER_WATER] = false;

	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_PREVENT] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_NEXT_MAP] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_WATER] = true;

	matrix[COLLIDER_PREVENT][COLLIDER_PREVENT] = false;
	matrix[COLLIDER_PREVENT][COLLIDER_WALL] = true;
	matrix[COLLIDER_PREVENT][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PREVENT][COLLIDER_NEXT_MAP] = false;
	matrix[COLLIDER_PREVENT][COLLIDER_WATER] = false;

	matrix[COLLIDER_NEXT_MAP][COLLIDER_PREVENT] = false;
	matrix[COLLIDER_NEXT_MAP][COLLIDER_WALL] = false;
	matrix[COLLIDER_NEXT_MAP][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_NEXT_MAP][COLLIDER_NEXT_MAP] = false;
	matrix[COLLIDER_NEXT_MAP][COLLIDER_WATER] = false;

	matrix[COLLIDER_WATER][COLLIDER_PREVENT] = false;
	matrix[COLLIDER_WATER][COLLIDER_WALL] = false;
	matrix[COLLIDER_WATER][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WATER][COLLIDER_NEXT_MAP] = false;

	matrix[COLLIDER_WATER][COLLIDER_WATER] = false;
}

j1Colliders::~j1Colliders()
{}

bool j1Colliders::Awake(pugi::xml_node& node)
{
	debug = node.child("debug").attribute("value").as_bool();

	return true;
}

bool j1Colliders::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	return true;
}

// Called before render is available
bool j1Colliders::Update(float dt)
{
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
		{
			continue;
		}
		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
			{
				continue;
			}
			c2 = colliders[k];

			if (c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->clld_type][c2->clld_type] && c1->callback)
				{
					c1->callback->OnCollision(c1, c2);
				}
				if (matrix[c2->clld_type][c1->clld_type] && c2->callback)
				{
					c2->callback->OnCollision(c2, c1);
				}
			}
		}
	}

	return true;
}

bool j1Colliders::PostUpdate()
{
	DebugDraw();

	return true;
}

void j1Colliders::DebugDraw()
{
	if (App->MInput->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		debug = !debug;
	}

	if (debug == false)
	{
		return;
	}

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->clld_type)
		{
		case COLLIDER_NONE:
			App->MRender->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha); //White
			break;
		case COLLIDER_WALL:
			App->MRender->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha); //Blue
			break;
		case COLLIDER_PLAYER:
			App->MRender->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha); //Green
			break;
		case COLLIDER_WATER:
			App->MRender->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha); //Red
			break;
		case COLLIDER_PREVENT:
			App->MRender->DrawQuad(colliders[i]->rect, 255, 255, 0, alpha);
			break;
		case COLLIDER_NEXT_MAP:
			App->MRender->DrawQuad(colliders[i]->rect, 255, 255, 0, alpha);
			break;
		}
	}
}

// Called before quitting
bool j1Colliders::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* j1Colliders::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}

	return ret;
}

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	bool ret = false;

	if (((rect.y + rect.h) > r.y && (r.y + r.h) > rect.y) && ((rect.x + rect.w) > r.x && (r.x + r.w) > rect.x))
	{
		ret = true;
	}

	return ret;
}