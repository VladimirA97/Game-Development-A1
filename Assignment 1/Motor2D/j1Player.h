#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "Animation.h"
#include "j1Module.h"

#include "j1Module.h"
//#include "Animation.h"
//#include "p2Point.h"
#include "j1App.h"
#include "j1Render.h"
//#include "j1Module.h"
#include <iostream>

struct SDL_Texture;

struct PlayerData
{
	iPoint			position;
	iPoint			velocity;
	iPoint			force_power;
	SDL_Rect		collider;
	Animation*      current_animation = nullptr;
	Animation       idle;
	Animation       left;
	Animation       right;
	Animation       jump_right;
	Animation       jump_left;
	SDL_Texture*	player_texture = nullptr;

	void Pushbacks();
};

class j1Player : public j1Module
{
public:
	j1Player();
	~j1Player();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void ToDraw();
	//void OnCollision(Collider* c1, Collider* c2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

private:
	p2SString str_folder;
	p2SString str_texture_path;
	PlayerData PlDt_Player;
};

#endif //__ModulePlayer_H__