//#include "p2Log.h"
//#include "j1App.h"
//#include "j1Textures.h"
//#include "j1Input.h"
//#include "j1Render.h"
//#include "j1Collision.h"
//#include "j1Map.h"
//#include "j1Player.h"
//#include "j1Audio.h"
//#include "j1Physics.h"
//#include "SDL/include/SDL_timer.h"
//
//#include<stdio.h>
//
//#define HORSE_AUDIO_DURATION 7000
//
//
//j1Player::j1Player()
//{
//}
//
//bool j1Player::Awake(pugi::xml_node& config)
//{
//	LOG("Loading player config");
//	width = config.child("size").attribute("width").as_int(20);
//	height = config.child("size").attribute("height").as_int(20);
//	jump_speed = config.child("acceleration").attribute("jump").as_float(-500);
//	acceleration = config.child("acceleration").attribute("horizontal").as_float(0.1);
//	max_speed = config.child("velocity").attribute("max").as_float(1.5);
//	gravity = config.child("gravity").attribute("value").as_float(0.03);
//
//	return true;
//}
//
//j1Player::~j1Player()
//{
//}
//
//// Load assets
//bool j1Player::Start()
//{
//	LOG("Loading player");
//	//create object
//
//	SDL_Rect rect;
//	rect.x = initial_x;
//	rect.y = initial_y;
//	rect.w = width;
//	rect.h = height;
//	graphics = App->tex->Load("textures/PilotSprites.png");
//	current_animation = &idle;
//	player = App->physics->Addobject(initial_x, initial_y, gravity, &rect, COLLIDER_PLAYER, this);
//	return true;
//}
//
//// Unload assets
//bool j1Player::CleanUp()
//{
//	LOG("Unloading player");
//	App->tex->UnLoad(graphics);
//
//	return true;
//}
//
//bool j1Player::PreUpdate()
//{
//	return true;
//}
//
//// Update: draw background
//bool j1Player::Update(float dt)
//{
//	//CONTROLS
//	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && player->velocity.x <max_speed)
//	{
//		player->acceleration.x = acceleration;
//		current_animation = &left;
//	}
//	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && player->velocity.x >-max_speed)
//	{
//		player->acceleration.x = -acceleration;
//		current_animation = &right;
//	}
//	if (player->velocity.x > max_speed || player->velocity.x < -max_speed)
//	{
//		player->acceleration.x = 0;
//	}
//
//	if ((App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
//		&& (App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE))
//	{
//		current_animation = &idle;
//		player->acceleration.x = -player->velocity.x / 10; //this stops the player in 10 frames
//	}
//	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT
//		&& App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
//	{
//		current_animation = &idle;
//		player->acceleration.x = -player->velocity.x / 10; //this stops the player in 10 frames
//		player->velocity.x = 0;
//	}
//
//	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && player->grounded && player->velocity.y < 0.5)
//	{
//		player->velocity.y = -jump_speed;
//		player->grounded = false;
//	}
//	//DEBUG FEATURES
//	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
//	{
//		App->map->change_map(0);
//	}
//	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
//	{
//		SetPosOrigin();
//	}
//	//aereal animations
//	if (player->grounded == false)
//	{
//		if (player->velocity.y < 0)
//		{
//			if (player->velocity.x < 0)
//				current_animation = &jumpsquatLeft;
//			if (player->velocity.x >= 0)
//				current_animation = &jumpsquatRight;
//		}
//		else
//		{
//			if (player->velocity.x < 0)
//				current_animation = &airborneLeft;
//			if (player->velocity.x >= 0)
//				current_animation = &airborneRight;
//		}
//	}
//	//blit
//	App->render->Blit(graphics, (int)player->position.x - 10, (int)player->position.y, &(current_animation->GetCurrentFrame()));
//
//	return true;
//}
//
//
//void j1Player::OnCollision(Collider* c1, Collider* c2)
//{
//	if (c1->type == COLLIDER_PLAYER &&c2->type == COLLIDER_NEXT_LEVEL)
//	{
//		App->map->next_level();
//	}
//	if (c1->type == COLLIDER_PLAYER &&c2->type == COLLIDER_LAVA)
//	{
//		SetPosOrigin();
//	}
//}
//
//bool j1Player::Save(pugi::xml_node& node) const
//{
//	pugi::xml_node pos = node.append_child("position");
//
//	pos.append_attribute("x") = player->position.x;
//	pos.append_attribute("y") = player->position.y;
//	return true;
//}
//
//bool j1Player::Load(pugi::xml_node& node)
//{
//	player->position.x = node.child("position").attribute("x").as_int();
//	player->position.y = node.child("position").attribute("y").as_int();
//	player->velocity.x = 0;
//	player->velocity.y = 0;
//	return true;
//}
//
//void j1Player::SetPosOrigin()
//{
//	player->position.x = initial_x;
//	player->position.y = initial_y;
//	player->velocity.x = 0;
//	player->velocity.y = 0;
//}
