#include "j1Physics.h"
#include "j1Collision.h"
#include "j1App.h"
#include "p2Log.h"

j1Physics::j1Physics()
{
}

bool j1Physics::Awake(pugi::xml_node* config)
{
	/*objects = new object[MAX_OBJECTS];

	memset(objects, 0, sizeof(object)*(MAX_OBJECTS));*/

	return true;
}

object* j1Physics::Addobject(float _x, float _y, float _gravity, SDL_Rect* collision, COLLIDER_TYPE TYPE, j1Module* Callback)
{
	object* ret = nullptr;

	for (uint i = 0; i < MAX_OBJECTS; ++i)//15 = MAX OBJECTS THAT SHOULD BE IN THE CONFIG FILE
	{
		if (objects[i] == nullptr)
		{
			ret = objects[i] = new object;
			break;
		}
	}

	ret->position.x = _x;
	ret->position.y = _y;
	ret->acceleration.x = 0;
	ret->acceleration.y = _gravity;

	ret->col = App->collision->AddCollider(*collision, TYPE, Callback);
	ret->predictor = App->collision->AddCollider(*collision, COLLIDER_FUTURE, this);

	ret->velocity.x = 0;
	ret->velocity.y = 0;

	return ret;
}

bool j1Physics::CleanUp()
{
	LOG("Freeing all objects");

	for (uint i = 0; i < MAX_OBJECTS; ++i)
	{
		if (objects[i] != nullptr)
		{
			delete objects[i];
			//objects[i] = nullptr;
		}
	}

	return true;
}

bool j1Physics::PreUpdate()
{
	//HERE U SET THE RECTANGLE FUTURE FOR THE ITEMS SO THEY COLLIDE WHEN THE TIME COMES AND YOU GET THE FUNCTION CALLED
	for (uint i = 0; i < MAX_OBJECTS; ++i)
	{
		if (objects[i] != nullptr)
			objects[i]->predictor->SetPos(objects[i]->position.x + objects[i]->velocity.x,
				                          objects[i]->position.y + objects[i]->velocity.y);
	}
	return true;
}

bool j1Physics::Update(float dt)
{
	//manage physics
	for (uint i = 0; i < MAX_OBJECTS; ++i)
	{
		if (objects[i] != nullptr)
		{
			objects[i]->velocity += objects[i]->acceleration;

			objects[i]->position += objects[i]->velocity;
		}
	}
	return true;
}

void j1Physics::OnCollision(Collider* c1, Collider*c2)
{
	//this means this is going to collide next frame
	if (c1->type == COLLIDER_FUTURE)
	{
		SDL_Rect result;
		SDL_bool _bool = SDL_IntersectRect(&c1->rect, &c2->rect, &result);
		if (_bool)//this means they collided 4 real
		{
			object* obj = GetObjectFromRect_predictor(&c1->rect);
			if (result.h == result.w)
			{
				obj->position.y = obj->predictor->rect.y - result.h;
				obj->position.x = obj->predictor->rect.x - result.w;
			}
			else if (result.h >= result.w)
			{
				if (c1->rect.x <c2->rect.x)
				{
					obj->position.x = obj->predictor->rect.x - result.w;
				}
				else
				{
					obj->position.x = obj->predictor->rect.x + result.w;
				}
				obj->velocity.x = 0;

			}
			else if (result.h < result.w)
			{
				if (c1->rect.y < c2->rect.y)
				{
					obj->position.y = obj->predictor->rect.y - result.h;
					obj->grounded = true;
				}
				else
				{
					obj->position.y = obj->predictor->rect.y + result.h;
				}
				obj->velocity.y = 0;
			}
		}
	}
};

bool j1Physics::PostUpdate()
{
	for (uint i = 0; i < MAX_OBJECTS; ++i)
	{
		if (objects[i] != nullptr)
		{
			objects[i]->col->SetPos(objects[i]->position.x, objects[i]->position.y);
		}
	}
	return true;
}

object* j1Physics::GetObjectFromRect_predictor(SDL_Rect* rectangle)
{
	for (uint i = 0; i < MAX_OBJECTS; ++i)
	{
		if (objects[i] != nullptr)
		{
			if (objects[i]->predictor->rect.x == rectangle->x&&
				objects[i]->predictor->rect.y == rectangle->y&&
				objects[i]->predictor->rect.w == rectangle->w&&
				objects[i]->predictor->rect.h == rectangle->h)
			{
				return objects[i];
			}
		}
	}
}