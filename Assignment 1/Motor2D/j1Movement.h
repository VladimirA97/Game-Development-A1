#ifndef __J1MOVEMENT_H__
#define __J1MOVEMENT_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Colliders.h"

#define MAX_OBJECTS 10

struct Body
{
	fPoint position;
	fPoint velocity;
	fPoint acceleration;
	Collider* collider;
	Collider* prevent;
	bool is_touching = false;
};

class j1Movement : public j1Module
{
public:
	j1Movement();
	~j1Movement() {};

	bool Awake(pugi::xml_node* config);
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	void OnCollision(Collider* c1, Collider* c2);
	bool CleanUp();

	Body* AddBody(float x, float y, float gravity, SDL_Rect* collision, COLLIDER_TYPE TYPE, j1Module* callback = NULL);
	Body* bodies[MAX_OBJECTS];
	Body* GetBodyRectPrevent(SDL_Rect* rectangle);
};
#endif

