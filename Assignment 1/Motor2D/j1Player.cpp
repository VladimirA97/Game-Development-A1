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
	idle.PushBack({ 0, 0, 53, 64 });
	idle.PushBack({ 53, 0, 53, 64 });
	idle.PushBack({ 106, 0, 53, 64 });
	idle.PushBack({ 159, 0, 53, 64 });
	idle.PushBack({ 212, 0, 53, 64 });
	idle.PushBack({ 265, 0, 53, 64 });
	idle.PushBack({ 318, 0, 53, 64 });
	idle.speed = 0.02f;
	idle.loop = true;

	//Animation right
	run_right.PushBack({ 0, 192, 53, 64 });
	run_right.PushBack({ 53, 192, 53, 64 });
	run_right.PushBack({ 106, 192, 53, 64 });
	run_right.PushBack({ 159, 192, 53, 64 });
	run_right.PushBack({ 212, 192, 53, 64 });
	run_right.PushBack({ 265, 192, 53, 64 });
	run_right.PushBack({ 318, 192, 53, 64 });
	run_right.speed = 0.02f;
	run_right.loop = true;

	//Animation left
	run_left.PushBack({ 0, 256, 53, 64 });
	run_left.PushBack({ 53, 256, 53, 64 });
	run_left.PushBack({ 106, 256, 53, 64 });
	run_left.PushBack({ 159, 256, 53, 64 });
	run_left.PushBack({ 212, 256, 53, 64 });
	run_left.PushBack({ 265, 256, 53, 64 });
	run_left.PushBack({ 318, 256, 53, 64 });
	run_left.speed = 0.02f;
	run_left.loop = true;

	//Animation jumps_right;
	jump_right.PushBack({ 0, 64, 53, 64 });
	jump_right.PushBack({ 53, 64, 53, 64 });
	jump_right.PushBack({ 106, 64, 53, 64 });
	jump_right.PushBack({ 159, 64, 53, 64 });
	jump_right.PushBack({ 212, 64, 53, 64 });
	jump_right.PushBack({ 265, 64, 53, 64 });
	jump_right.PushBack({ 318, 64, 53, 64 });
	jump_right.speed = 0.001f;
	jump_right.loop = false;

	//Animation jump_left;
	jump_left.PushBack({ 0, 192, 53, 64 });
	jump_left.PushBack({ 53, 192, 53, 64 });
	jump_left.PushBack({ 106, 192, 53, 64 });
	jump_left.PushBack({ 159, 192, 53, 64 });
	jump_left.PushBack({ 212, 192, 53, 64 });
	jump_left.PushBack({ 265, 192, 53, 64 });
	jump_left.PushBack({ 318, 192, 53, 64 });
	jump_left.speed = 0.001f;
	jump_left.loop = false;
}

bool j1Player::Awake(pugi::xml_node& config)
{
	LOG("Loading player config");
	clld_width = config.child("dimentions").attribute("clld_width").as_int(30);
	clld_height = config.child("dimentions").attribute("clld_height").as_int(60);

	jump_speed = config.child("acceleration").attribute("jump").as_float(1.15);
	acceleration = config.child("acceleration").attribute("horizontal").as_float(0.015);
	velocity = config.child("velocity").attribute("value").as_float(0.5);
	gravity = config.child("gravity").attribute("value").as_float(0.0075);

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
	rect.x = original_x;
	rect.y = original_y;
	rect.w = clld_width;
	rect.h = clld_height;

	player_graphics = App->tex->Load("textures/Player Spritesheet2.png");
	current_animation = &idle;
	player = App->physics->Addobject(original_x, original_y, gravity, &rect, COLLIDER_PLAYER, this);
	return true;
}

//Unload assets
bool j1Player::CleanUp()
{
	LOG("Unloading player");
	App->tex->UnLoad(player_graphics);

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
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && player->velocity.x <velocity)
	{
		player->acceleration.x = acceleration;
		current_animation = &run_right;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && player->velocity.x >-velocity)
	{
		player->acceleration.x = -acceleration;
		current_animation = &run_left;
	}
	if (player->velocity.x > velocity || player->velocity.x < -velocity)
	{
		player->acceleration.x = 0;
	}

	if ((App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
		&& (App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE))
	{
		current_animation = &idle;
		player->acceleration.x = -player->velocity.x;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT
		&& App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		current_animation = &idle;
		player->acceleration.x = -player->velocity.x;
		player->velocity.x = 0;
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && player->grounded && player->velocity.y < 0.5)
	{
		current_animation = &jump_right;
		player->velocity.y = -jump_speed;
		player->grounded = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{

			player->velocity.y -= -1.0; //stack to the floor
	}

	//F1: Go to Map 1
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		App->map->change_map(0);
	}

	//F1: Go to Map 2
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		App->map->change_map(1);
	}

	//F2: Reset current Map 
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
				current_animation = &jump_left;
			if (player->velocity.x > 0)
				current_animation = &jump_right;
		}
	}

	//blit
	App->render->Blit(player_graphics, (int)player->position.x - 10, (int)player->position.y, &(current_animation->GetCurrentFrame()));

	return true;
}

void j1Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == COLLIDER_PLAYER &&c2->type == COLLIDER_NEXT_LEVEL)
	{
		App->map->next_level();
	}

	if (c1->type == COLLIDER_PLAYER &&c2->type == COLLIDER_WATER)
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
	player->position.x = original_x;
	player->position.y = original_y;
	player->velocity.x = 0;
	player->velocity.y = 0;
}