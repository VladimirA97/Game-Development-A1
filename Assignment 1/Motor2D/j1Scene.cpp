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
#include "j1Physics.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("Test");
	background_rect.w = 3000;
	background_rect.h = 750;
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;
	
	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->map->Load(App->map->current_map.GetString());
	background_text = App->tex->Load("maps/BG1.png");
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
	uint win;
	uint i;
	App->win->GetWindowSize(win, i);
	App->render->camera.x = -App->player->player->position.x + win / 2;

	if (App->render->camera.x < win / 2)
	{
		App->render->camera.x = 0;
	}

	if (-App->render->camera.x >((App->map->data.width*App->map->data.tile_width) - win))
	{
		App->render->camera.x = -(App->map->data.width*App->map->data.tile_width) + win;
	}

	//---------------------
	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame();

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame();

	/*if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 1;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 1;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 1;*/

	//App->render->Blit(img, 0, 0);
	
	App->render->Blit(background_text, App->render->camera.x - 500, App->render->camera.y, &background_rect, 0.01f);
	App->map->Draw();

	// TODO 7: Set the window title like
	// "Map:%dx%d Tiles:%dx%d Tilesets:%d"
	/*p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count());*/
	p2SString title("Platformer 2D - Bondache");

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
	if (background_text != nullptr)
	{
		App->tex->UnLoad(background_text);
		background_text = nullptr;
	}
	return true;
}
