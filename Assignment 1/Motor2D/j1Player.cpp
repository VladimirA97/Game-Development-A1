#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Player.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Render.h"


#include <iostream>
//#include "j1App.h"
//#include "j1Textures.h"
#include "j1Input.h"
//#include "j1Render.h"
//#include "j1Player.h"
#include "j1Window.h"

//#include "p2Defs.h"
//#include "p2Log.h"

j1Player::j1Player():j1Module()
{
	name.create("Player");
}

j1Player::~j1Player()
{}

bool j1Player::Awake(pugi::xml_node& config)
{
	bool ret = true;
	LOG("Init Player config");
	str_folder.create(config.child("folder").child_value());
	str_texture_path = config.child("sprite").attribute("content").as_string();

	return ret;
}

// Load assets
bool j1Player::Start()
{
	bool ret = true;
	LOG("Loading player textures");

	PlDt_Player.Pushbacks();

	PlDt_Player.velocity = { 0,0 };
	PlDt_Player.force_power = { 0,0 };
	PlDt_Player.current_animation = &PlDt_Player.idle;
	for (p2List_item<ObjectsGroup*>* curr_obj = App->map->data.ObGr_objects.start; curr_obj; curr_obj = curr_obj->next)
	{
		if (curr_obj->data->name == ("Collisions"))
		{
			p2List_item<ObjectsData*>* objdata = curr_obj->data->objects.start;
			while (objdata)
			{
				if (objdata->data->name == ("Player"))
				{
					PlDt_Player.collider.h = objdata->data->height;
					PlDt_Player.collider.w = objdata->data->width;
					PlDt_Player.collider.x = objdata->data->x;
					PlDt_Player.collider.y = objdata->data->y;
				}
				else if (objdata->data->name == ("Start"))
				{
					PlDt_Player.position = { objdata->data->x, objdata->data->y };
				}

				objdata = objdata->next;
			}
		}
	}
	PlDt_Player.player_texture = App->tex->Load(PATH(str_folder.GetString(), str_texture_path.GetString()));
	
	return true;
}

bool j1Player::PreUpdate()
{
	return true;
}

bool j1Player::Update(float dt)
{
	return false;
}

bool j1Player::PostUpdate()
{
	return false;
}

void j1Player::ToDraw()
{
	App->render->Blit(PlDt_Player.player_texture, PlDt_Player.position.x, PlDt_Player.position.y, &(PlDt_Player.current_animation->GetCurrentFrame()));
}

bool j1Player::CleanUp()
{
	bool ret = true;

	//App->tex->UnLoad(player_texture);
	//player_texture = nullptr;

	return ret;
}

//void j1Player::OnCollision(Collider * c1, Collider * c2)
//{
//}

bool j1Player::Load(pugi::xml_node& data)
{
	//position.x = data.child("player").attribute("x").as_int();
	//position.y = data.child("player").attribute("y").as_int();

	return true;
}

bool j1Player::Save(pugi::xml_node& data) const
{
	//pugi::xml_node player = data.append_child("player");

	//player.append_attribute("x") = position.x;
	//player.append_attribute("y") = position.y;

	return true;
}

void PlayerData::Pushbacks()
{
	idle.PushBack({ 5, 64, 21, 32 });//x - y - w - h
	idle.PushBack({ 37, 64, 21, 32 });
	idle.PushBack({ 68, 64, 21, 32 });
	idle.PushBack({ 101, 64, 21, 32 });
	idle.speed = 0.1f;
}