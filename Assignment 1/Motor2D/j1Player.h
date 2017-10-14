#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"

struct object;
struct SDL_Texture;
struct Collider;

class j1Player :
	public j1Module
{
public:
	j1Player();
	~j1Player();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);
	bool Awake(pugi::xml_node& config);

	void SetPosOrigin();// sets player to the start of the map

	bool Save(pugi::xml_node&) const;
	bool Load(pugi::xml_node&);

	void InitialPos()
	{
		position.x = 0;
		position.y = 0;
	}

public:

	SDL_Texture* graphics = nullptr;
	SDL_Texture* sprites = nullptr;

	Collider* collider;
	fPoint position;

	Animation* current_animation = nullptr;
	Animation idle;
	Animation left;
	Animation right;
	Animation jumpsquatRight;
	Animation jumpsquatLeft;

	//default values are added if everything that sets em up is going horrible
	int lifes = 3;
	int initial_x = 10;
	int initial_y = 10;
	int width = 53;
	int height = 64;
	float jump_speed = -5;
	float acceleration = 0.1;
	float max_speed = 1;
	float gravity = 0.01;

	object* player;
};
#endif
