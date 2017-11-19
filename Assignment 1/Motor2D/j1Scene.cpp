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
#include "j1Movement.h"
#include "Brofiler/Brofiler.h"

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
	App->MPlayer->Enable();
	App->MColliders->Enable();
	App->MAudio->Enable();

	App->MMap->Load(App->MMap->maps[App->MMap->id_map].GetString());
	
	if (App->MMap->id_map == 0)
	{
		App->MAudio->PlayMusic("audio/music/First Map.ogg");
	}
	else if (App->MMap->id_map == 1)
	{
		App->MAudio->PlayMusic("audio/music/Second Map.ogg");
	}

	background_text = App->MTextures->Load("maps/BG1.png");

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
	BROFILER_CATEGORY("Scene_Update", Profiler::Color::Red);

	uint win;
	uint i;
	App->MWindow->GetWindowSize(win, i);
	App->MRender->camera.x = -App->MPlayer->player->position.x + win / 2;

	if (App->MRender->camera.x < win / 2)
	{
		App->MRender->camera.x = 0;
	}

	if (-App->MRender->camera.x >((App->MMap->data.width*App->MMap->data.tile_width) - win))
	{
		App->MRender->camera.x = -(App->MMap->data.width*App->MMap->data.tile_width) + win;
	}

	if (App->MInput->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	if (App->MInput->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();
	
	App->MRender->Blit(background_text, App->MRender->camera.x - 500, App->MRender->camera.y, &background_rect, 0.05f);
	App->MMap->Draw();

	//p2SString title("Escape");
	//App->MWindow->SetTitle(title.GetString());

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->MInput->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		App->MScene->CleanUp();
		exit(0);
		//ret = false;
	}
	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	if (background_text != nullptr)
	{
		App->MTextures->UnLoad(background_text);
		background_text = nullptr;
	}

	App->MPlayer->Disable();
	App->MColliders->Disable();
	App->MAudio->Disable();

	return true;
}