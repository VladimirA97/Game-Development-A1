#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Player.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("Scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	for (pugi::xml_node curr_map = config.child("nameMap"); curr_map; curr_map = curr_map.next_sibling("nameMap"))
	{
		p2SString* data = new p2SString;

		data->create(curr_map.attribute("name").as_string());
		str_MapName.add(data);
	}

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	LOG("Loading music");
	App->audio->PlayMusic("audio/music/TES V Skyrim - Dovahkiin (5E7EN Rave Remix).ogg");

	App->map->is_Map1 = true;
	bool ret = App->map->Load(str_MapName.start->data->GetString());
	LOG("Map name: %s", str_MapName.start->data->GetString());

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame();

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y += 1;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x -= 1;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x += 1;

	////Fade to black
	//if (App->player->IsEnabled() == false)
	//	lost = true;

	//if (App->render->camera.y >= -1200 && fading == false && App->fade->GetFadeState() == false || App->enemies->boss_death == true && fading == false && App->fade->GetFadeState() == false)
	//{
	//	winning = true;
	//	fading = true;
	//	win_timer_start = SDL_GetTicks();
	//}

	//App->render->Blit(img, 0, 0);
	App->map->Draw();

	// TODO 7: Set the window title like
	// "Map:%dx%d Tiles:%dx%d Tilesets:%d"
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
					App->map->data.MapDa_width, App->map->data.MapDa_height,
					App->map->data.MapDa_tile_width, App->map->data.MapDa_tile_height,
					App->map->data.MapDa_tilesets.count());

	App->win->SetTitle(title.GetString());

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
