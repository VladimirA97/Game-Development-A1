#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "j1Player.h"
#include "j1Audio.h"
#include "j1Physics.h"
#include "SDL/include/SDL_timer.h"

#include <stdio.h>

j1Player::j1Player()
{
	name.create("player");
	//Animation idle;
	idle.PushBack({ 0, 0, 113, 138 });
	idle.PushBack({ 113, 0, 113, 138 });
	idle.PushBack({ 226, 0, 113, 138 });
	idle.PushBack({ 339, 0, 113, 138 });
	idle.PushBack({ 452, 0, 113, 138 });
	idle.PushBack({ 565, 0, 113, 138 });
	idle.PushBack({ 678, 0, 113, 138 });
	idle.speed = 0.02f;
	idle.loop = true;

	//Animation right
	right.PushBack({ 0, 552, 113, 138 });
	right.PushBack({ 113, 552, 113, 138 });
	right.PushBack({ 226, 552, 113, 138 });
	right.PushBack({ 339, 552, 113, 138 });
	right.PushBack({ 452, 552, 113, 138 });
	right.PushBack({ 565, 552, 113, 138 });
	right.PushBack({ 678, 552, 113, 138 });
	right.speed = 0.02f;
	right.loop = true;

	//Animation left
	left.PushBack({ 0, 414, 113, 138 });
	left.PushBack({ 113, 414, 113, 138 });
	left.PushBack({ 226, 414, 113, 138 });
	left.PushBack({ 339, 414, 113, 138 });
	left.PushBack({ 452, 414, 113, 138 });
	left.PushBack({ 565, 414, 113, 138 });
	left.PushBack({ 678, 414, 113, 138 });
	left.speed = 0.02f;
	left.loop = true;

	//Animation jumpsquatRight;
	jumpsquatRight.PushBack({ 0, 138, 113, 138 });
	jumpsquatRight.PushBack({ 113, 138, 113, 138 });
	jumpsquatRight.PushBack({ 226, 138, 113, 138 });
	jumpsquatRight.PushBack({ 339, 138, 113, 138 });
	jumpsquatRight.PushBack({ 452, 138, 113, 138 });
	jumpsquatRight.PushBack({ 565, 138, 113, 138 });
	jumpsquatRight.PushBack({ 678, 138, 113, 138 });
	jumpsquatRight.speed = 0.001f;
	jumpsquatRight.loop = false;

	//Animation jumpsquatLeft;
	jumpsquatLeft.PushBack({ 0, 276, 113, 138 });
	jumpsquatLeft.PushBack({ 113, 276, 113, 138 });
	jumpsquatLeft.PushBack({ 226, 276, 113, 138 });
	jumpsquatLeft.PushBack({ 339, 276, 113, 138 });
	jumpsquatLeft.PushBack({ 452, 276, 113, 138 });
	jumpsquatLeft.PushBack({ 565, 276, 113, 138 });
	jumpsquatLeft.PushBack({ 678, 276, 113, 138 });
	jumpsquatLeft.speed = 0.001f;
	jumpsquatLeft.loop = false;

	//Animation airborneRight;
	airborneRight.PushBack({ 0, 138, 113, 138 });
	airborneRight.PushBack({ 113, 138, 113, 138 });
	airborneRight.PushBack({ 226, 138, 113, 138 });
	airborneRight.PushBack({ 339, 138, 113, 138 });
	airborneRight.PushBack({ 452, 138, 113, 138 });
	airborneRight.PushBack({ 565, 138, 113, 138 });
	airborneRight.PushBack({ 678, 138, 113, 138 });
	airborneRight.speed = 0.001f;
	airborneRight.loop = false;

	//Animation airborneLeft;
	airborneLeft.PushBack({ 0, 276, 113, 138 });
	airborneLeft.PushBack({ 113, 276, 113, 138 });
	airborneLeft.PushBack({ 226, 276, 113, 138 });
	airborneLeft.PushBack({ 339, 276, 113, 138 });
	airborneLeft.PushBack({ 452, 276, 113, 138 });
	airborneLeft.PushBack({ 565, 276, 113, 138 });
	airborneLeft.PushBack({ 678, 276, 113, 138 });
	airborneLeft.speed = 0.001f;
	airborneLeft.loop = false;
}

bool j1Player::Awake(pugi::xml_node& config)
{
	LOG("Loading player config");
	width = config.child("size").attribute("width").as_int(20);
	height = config.child("size").attribute("height").as_int(20);

	jump_speed = config.child("acceleration").attribute("jump").as_float(-500);
	acceleration = config.child("acceleration").attribute("horizontal").as_float(0.1);
	max_speed = config.child("velocity").attribute("max").as_float(1.5);
	gravity = config.child("gravity").attribute("value").as_float(0.03);

	return true;
}

j1Player::~j1Player()
{
}

//Load assets
bool j1Player::Start()
{
	LOG("Loading player");
	//create object

	SDL_Rect rect;
	rect.x = initial_x;
	rect.y = initial_y;
	rect.w = width;
	rect.h = height;

	graphics = App->tex->Load("textures/Player Spritesheet.png");
	current_animation = &idle;
	player = App->physics->Addobject(initial_x, initial_y, gravity, &rect, COLLIDER_PLAYER, this);
	return true;
}

//Unload assets
bool j1Player::CleanUp()
{
	LOG("Unloading player");
	App->tex->UnLoad(graphics);

	return true;
}

bool j1Player::PreUpdate()
{
	return true;
}

//Update: draw background
bool j1Player::Update(float dt)
{
	//CONTROLS
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && player->velocity.x <max_speed)
	{
		player->acceleration.x = acceleration;
		current_animation = &left;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && player->velocity.x >-max_speed)
	{
		player->acceleration.x = -acceleration;
		current_animation = &right;
	}
	if (player->velocity.x > max_speed || player->velocity.x < -max_speed)
	{
		player->acceleration.x = 0;
	}

	if ((App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
		&& (App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE))
	{
		current_animation = &idle;
		player->acceleration.x = -player->velocity.x / 10; //this stops the player in 10 frames
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT
		&& App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		current_animation = &idle;
		player->acceleration.x = -player->velocity.x / 10; //this stops the player in 10 frames
		player->velocity.x = 0;
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && player->grounded && player->velocity.y < 0.5)
	{
		player->velocity.y = -jump_speed;
		player->grounded = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		player->velocity.y = -0.5;
	}

	//DEBUG FEATURES
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		App->map->change_map(0);
	}
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		SetPosOrigin();
	}
	//aereal animations
	if (player->grounded == false)
	{
		if (player->velocity.y < 0)
		{
			if (player->velocity.x < 0)
				current_animation = &jumpsquatLeft;
			if (player->velocity.x >= 0)
				current_animation = &jumpsquatRight;
		}
		else
		{
			if (player->velocity.x < 0)
				current_animation = &airborneLeft;
			if (player->velocity.x >= 0)
				current_animation = &airborneRight;
		}
	}
	//blit
	App->render->Blit(graphics, (int)player->position.x - 35, (int)player->position.y - 25, &(current_animation->GetCurrentFrame()));

	return true;
}


void j1Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == COLLIDER_PLAYER &&c2->type == COLLIDER_NEXT_LEVEL)
	{
		App->map->next_level();
	}
	if (c1->type == COLLIDER_PLAYER &&c2->type == COLLIDER_LAVA)
	{
		SetPosOrigin();
	}
}

bool j1Player::Save(pugi::xml_node& node) const
{
	pugi::xml_node pos = node.append_child("position");

	pos.append_attribute("x") = player->position.x;
	pos.append_attribute("y") = player->position.y;
	pos.append_attribute("current_map") = App->map->index_map;

	return true;
}

bool j1Player::Load(pugi::xml_node& node)
{
	App->map->change_map(node.child("position").attribute("current_map").as_uint());
	player->position.x = node.child("position").attribute("x").as_int();
	player->position.y = node.child("position").attribute("y").as_int();
	player->velocity.x = 0;
	player->velocity.y = 0;

	return true;
}

void j1Player::SetPosOrigin()
{
	player->position.x = initial_x;
	player->position.y = initial_y;
	player->velocity.x = 0;
	player->velocity.y = 0;
}
