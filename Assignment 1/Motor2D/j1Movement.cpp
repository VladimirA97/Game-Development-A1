#include "j1Movement.h"
#include "j1Colliders.h"
#include "j1App.h"
#include "p2Log.h"

j1Movement::j1Movement()
{
	for (uint i = 0; i < MAX_OBJECTS; ++i)
	{
		bodies[i] = nullptr;
	}
}

bool j1Movement::Awake(pugi::xml_node* config)
{
	return true;
}

Body* j1Movement::AddBody(float x, float y, float gravity, SDL_Rect* collider, COLLIDER_TYPE CLLD_TYPE, j1Module* Callback)
{
	Body* body = nullptr;

	for (uint i = 0; i < MAX_OBJECTS; ++i)
	{
		if (bodies[i] == nullptr)
		{
			body = bodies[i] = new Body;
			break;
		}
	}

	body->position.x = x;
	body->position.y = y;
	body->acceleration.x = 0;
	body->acceleration.y = gravity;

	body->collider = App->Colliders->AddCollider(*collider, CLLD_TYPE, Callback);
	body->prevent = App->Colliders->AddCollider(*collider, COLLIDER_PREVENT, this);

	body->velocity.x = 0;
	body->velocity.y = 0;

	return body;
}

bool j1Movement::CleanUp()
{
	LOG("Freeing all bodies");

	for (uint i = 0; i < MAX_OBJECTS; ++i)
	{
		if (bodies[i] != nullptr)
		{
			if (bodies[i]->collider != nullptr)
			{
				bodies[i]->collider->to_delete = true;
			}
			if (bodies[i]->prevent != nullptr)
			{
				bodies[i]->prevent->to_delete = true;
			}
			delete bodies[i];
		}
	}

	return true;
}

bool j1Movement::PreUpdate()
{
	for (uint i = 0; i < MAX_OBJECTS; ++i)
	{
		if (bodies[i] != nullptr)
		{
			bodies[i]->prevent->SetPosition(
				bodies[i]->position.x + bodies[i]->velocity.x,
				bodies[i]->position.y + bodies[i]->velocity.y);
		}
	}

	return true;
}

bool j1Movement::Update(float dt)
{
	for (uint i = 0; i < MAX_OBJECTS; ++i)
	{
		if (bodies[i] != nullptr)
		{
			bodies[i]->velocity += bodies[i]->acceleration;
			bodies[i]->position += bodies[i]->velocity;
		}
	}

	return true;
}

Body* j1Movement::GetBodyRectPrevent(SDL_Rect* rectangle)
{
	for (uint i = 0; i < MAX_OBJECTS; ++i)
	{
		if (bodies[i] != nullptr)
		{
			if (bodies[i]->prevent->rect.x == rectangle->x &&
				bodies[i]->prevent->rect.y == rectangle->y &&
				bodies[i]->prevent->rect.w == rectangle->w &&
				bodies[i]->prevent->rect.h == rectangle->h)
			{
				return bodies[i];
			}
		}
	}
}

void j1Movement::OnCollision(Collider* c1, Collider*c2)
{
	if (c1->clld_type == COLLIDER_PREVENT)
	{
		SDL_Rect result;
		if (SDL_bool _bool = SDL_IntersectRect(&c1->rect, &c2->rect, &result))
		{
			Body* body = GetBodyRectPrevent(&c1->rect);
			if (result.h == result.w)
			{
				body->position.y = body->prevent->rect.y - result.h;
				body->position.x = body->prevent->rect.x - result.w;
			}
			else if (result.h >= result.w)
			{
				if (c1->rect.x <c2->rect.x)
				{
					body->position.x = body->prevent->rect.x - result.w;
				}
				else
				{
					body->position.x = body->prevent->rect.x + result.w;
				}
				body->velocity.x = 0;

			}
			else if (result.h < result.w)
			{
				if (c1->rect.y < c2->rect.y)
				{
					body->position.y = body->prevent->rect.y - result.h;
					body->is_touching = true;
				}
				else
				{
					body->position.y = body->prevent->rect.y + result.h;
				}
				body->velocity.y = 0;
			}
		}
	}
};

bool j1Movement::PostUpdate()
{
	for (uint i = 0; i < MAX_OBJECTS; ++i)
	{
		if (bodies[i] != nullptr)
		{
			bodies[i]->collider->SetPosition(bodies[i]->position.x, bodies[i]->position.y);
		}
	}

	return true;
}