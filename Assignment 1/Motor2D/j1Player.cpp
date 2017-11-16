#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Colliders.h"
#include "j1Map.h"
#include "j1Player.h"
#include "j1Audio.h"
#include "j1Movement.h"
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
	idle.speed = 0.5f;
	idle.loop = true;

	//Animation right
	run_right.PushBack({ 0, 192, 53, 64 });
	run_right.PushBack({ 53, 192, 53, 64 });
	run_right.PushBack({ 106, 192, 53, 64 });
	run_right.PushBack({ 159, 192, 53, 64 });
	run_right.PushBack({ 212, 192, 53, 64 });
	run_right.PushBack({ 265, 192, 53, 64 });
	run_right.PushBack({ 318, 192, 53, 64 });
	run_right.speed = 0.5f;
	run_right.loop = true;

	//Animation left
	run_left.PushBack({ 0, 256, 53, 64 });
	run_left.PushBack({ 53, 256, 53, 64 });
	run_left.PushBack({ 106, 256, 53, 64 });
	run_left.PushBack({ 159, 256, 53, 64 });
	run_left.PushBack({ 212, 256, 53, 64 });
	run_left.PushBack({ 265, 256, 53, 64 });
	run_left.PushBack({ 318, 256, 53, 64 });
	run_left.speed = 0.5f;
	run_left.loop = true;

	//Animation jumps_right;
	jump_right.PushBack({ 318, 64, 53, 64 });
	//jump_right.PushBack({ 53, 64, 53, 64 });
	//jump_right.PushBack({ 106, 64, 53, 64 });
	//jump_right.PushBack({ 159, 64, 53, 64 });
	//jump_right.PushBack({ 212, 64, 53, 64 });
	//jump_right.PushBack({ 265, 64, 53, 64 });
	//jump_right.PushBack({ 318, 64, 53, 64 });
	jump_right.speed = 0.5f;
	jump_right.loop = false;

	//Animation jump_left;
	jump_left.PushBack({ 318, 128, 53, 64 });
	//jump_left.PushBack({ 53, 192, 53, 64 });
	//jump_left.PushBack({ 106, 192, 53, 64 });
	//jump_left.PushBack({ 159, 192, 53, 64 });
	//jump_left.PushBack({ 212, 192, 53, 64 });
	//jump_left.PushBack({ 265, 192, 53, 64 });
	//jump_left.PushBack({ 318, 192, 53, 64 });
	jump_left.speed = 0.5f;
	jump_left.loop = false;
}

bool j1Player::Awake(pugi::xml_node& config)
{
	LOG("Loading player config");
	clld_width = config.child("dimentions").attribute("clld_width").as_int(30);
	clld_height = config.child("dimentions").attribute("clld_height").as_int(60);

	acc_y = config.child("acceleration").attribute("acc_y").as_float(1.15);
	acc_x = config.child("acceleration").attribute("acc_x").as_float(0.015);
	velocity = config.child("velocity").attribute("value").as_float(0.5);
	gravity = config.child("gravity").attribute("value").as_float(0.0075);

	return true;
}

j1Player::~j1Player()
{}

//Load assets
bool j1Player::Start()
{
	LOG("Loading player");

	SDL_Rect rect;
	rect.x = original_x;
	rect.y = original_y;
	rect.w = clld_width;
	rect.h = clld_height;

	player_graphics = App->MTextures->Load("textures/Player Spritesheet2.png");
	map1_done_text = App->MTextures->Load("maps/MAP1End.png");
	map2_done_text = App->MTextures->Load("maps/MAP2End.png");
	dark_text = App->MTextures->Load("maps/dark.png");

	current_animation = &idle;
	player = App->MMovement->AddBody(original_x, original_y, gravity, &rect, COLLIDER_PLAYER, this);

	//Effects
	jump_fx = App->MAudio->LoadFx("audio/effects/jump.wav");
	scream_fx = App->MAudio->LoadFx("audio/effects/scream.wav");

	return true;
}

//Unload assets
bool j1Player::CleanUp()
{
	LOG("Unloading player");
	App->MTextures->UnLoad(player_graphics);

	if (map1_done_text != nullptr)
	{
		App->MTextures->UnLoad(map1_done_text);
		map1_done_text = nullptr;
	}

	if (map2_done_text != nullptr)
	{
		App->MTextures->UnLoad(map2_done_text);
		map2_done_text = nullptr;
	}
	return true;
}

bool j1Player::PreUpdate()
{
	return true;
}

bool j1Player::Update(float dt)
{
	//WASD Movement
	if (finishedMap1 == false || !finishedMap2 == false)
	{
		if (App->MInput->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && player->velocity.x < velocity) {
			player->acceleration.x = acc_x;
			current_animation = &run_right;
		}

		if (App->MInput->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && player->velocity.x > -velocity) {
			player->acceleration.x = -acc_x;
			current_animation = &run_left;
		}

		if (player->velocity.x > velocity || player->velocity.x < -velocity) {
			player->acceleration.x = 0;
		}

		if ((App->MInput->GetKey(SDL_SCANCODE_D) == KEY_IDLE) && (App->MInput->GetKey(SDL_SCANCODE_A) == KEY_IDLE)) {
			current_animation = &idle;
			player->acceleration.x = -player->velocity.x;
		}

		if (App->MInput->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->MInput->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			current_animation = &idle;
			player->acceleration.x = -player->velocity.x;
			player->velocity.x = 0;
		}

		if (App->MInput->GetKey(SDL_SCANCODE_W) == KEY_DOWN && player->is_touching && player->velocity.y < 0.5) {
			current_animation = &idle;
			player->velocity.y = -acc_y;
			player->is_touching = false;
			//Effect
			App->MAudio->PlayFx(jump_fx);
		}

		if (player->is_touching == false)
		{
			if (player->velocity.y < 0)
			{
				if (player->velocity.x < 0) {
					current_animation = &jump_left;
				}
				if (player->velocity.x > 0) {
					current_animation = &jump_right;
				}
			}
		}

		//Stack to the floor
		if (App->MInput->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			player->velocity.y = +5.0;
		}
	}
	//F10: Switch to God mode  ---------------------------
	/*if (App->MInput->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		godmode = true;



	}*/

	//F1: Move to Map 1 ----------------------------------
	if (App->MInput->GetKey(SDL_SCANCODE_F1) == KEY_DOWN){
		App->MMap->switch_map(0);
		velocity = 5.0f;
	}
	//F1: Reset Map 1
	if (App->MInput->GetKey(SDL_SCANCODE_F2) == KEY_DOWN){
		SetPosOrigin();
	}
	//F1: Move to Map 2
	if (App->MInput->GetKey(SDL_SCANCODE_F11) == KEY_DOWN){
		App->MMap->switch_map(1);
		velocity = 5.0f;
	}
	
	App->MRender->Blit(player_graphics, (int)player->position.x - 10, (int)player->position.y, &(current_animation->GetCurrentFrame()));
	App->MRender->Blit(dark_text, (int)player->position.x - 1137, (int)player->position.y - 938);
	
	//End of map checking ---------------------------------
	if (finishedMap1 == true)
	{
		App->MRender->Blit(map1_done_text, App->MPlayer->position.x, App->MPlayer->position.y, NULL, 0.0f);
		
		if (App->MInput->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
		{
			App->MMap->switch_map(1);
			velocity = 5.0f;
		}
		App->MPlayer->finishedMap1 = false;
	}
	else if (finishedMap2 == true)
	{
		App->MRender->Blit(map2_done_text, App->MPlayer->position.x, App->MPlayer->position.y, NULL, 0.0f);

		if (App->MInput->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
		{
			App->MMap->switch_map(0);
			velocity = 5.0f;
		}
		else if (App->MInput->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			exit(0);
		}
		App->MPlayer->finishedMap2 = false;
	}

	return true;
}

void j1Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->clld_type == COLLIDER_PLAYER && c2->clld_type == COLLIDER_WATER)
	{
		App->MRender->Blit(dark_text, (int)player->position.x - 1137, (int)player->position.y - 938);
		//Effect
		App->MAudio->PlayFx(scream_fx);
		SetPosOrigin();
	}

	//End of map checking
	if (c1->clld_type == COLLIDER_PLAYER && c2->clld_type == COLLIDER_NEXT_MAP && App->MMap->id_map == 0)
	{
		finishedMap1 = true;
		velocity = 0.0f;
	}
	else if (c1->clld_type == COLLIDER_PLAYER && c2->clld_type == COLLIDER_NEXT_MAP && App->MMap->id_map == 1)
	{
		finishedMap2 = true;
		velocity = 0.0f;
	}
}

bool j1Player::Save(pugi::xml_node& node) const
{
	pugi::xml_node pos = node.append_child("position");

	pos.append_attribute("x") = player->position.x;
	pos.append_attribute("y") = player->position.y;
	pos.append_attribute("current_map") = App->MMap->id_map;

	return true;
}

bool j1Player::Load(pugi::xml_node& node)
{
	App->MMap->switch_map(node.child("position").attribute("current_map").as_uint());
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