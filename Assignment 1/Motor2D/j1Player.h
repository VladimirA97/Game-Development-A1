#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"

struct Body;
struct SDL_Texture;
struct Collider;

class j1Player : public j1Module
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

	void SetPosOrigin();

	bool Save(pugi::xml_node&) const;
	bool Load(pugi::xml_node&);

	void InitialPos()
	{
		position.x = 0;
		position.y = 0;
	}

public:

	SDL_Texture* player_graphics = nullptr;
	SDL_Texture* map1_done_text = nullptr;
	bool finishedMap1 = false;

	SDL_Texture* map2_done_text = nullptr;
	bool finishedMap2 = false;
	bool beforeMap1 = true;

	SDL_Texture* dark_text = nullptr;

	Collider* collider;
	fPoint position;

	Animation* current_animation = nullptr;
	Animation idle;
	Animation run_right;
	Animation run_left;
	Animation jump_right;
	Animation jump_left;

	//default values are added if everything that sets em up is going horrible
	int original_x = 10;
	int original_y = 10;
	int clld_width = 30;
	int clld_height = 60;
	float acc_y = 15;
	float acc_x = 5;
	float velocity = 5.0;
	float gravity = 0.75;

	Body* player;

public:
	uint jump_fx;
	uint scream_fx;
private:
	/*bool godmode = false;*/
};
#endif
