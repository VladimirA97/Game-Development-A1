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
#include "j1Fonts.h"
#include "j1Gui.h"
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

//---------------------------------------------------------------------
	//debug_tex = App->MTextures->Load("maps/path2.png");
	//background = App->MTextures->Load("textures/Homework/wow ui/login_background720.png");
	//upper = App->MTextures->Load("textures/Homework/wow ui/login_upper.png");
	//greyButtonWOW = App->MTextures->Load("textures/Homework/wow ui/BUTTONS/UI-DialogBox-Button-Disabled.png");
	//redButtonWOW = App->MTextures->Load("textures/Homework/wow ui/BUTTONS/UI-DialogBox-Button-Down.png");

	//// Create the banner (rect {485, 829, 328, 103}) and the text "Hello World"
	//atlasRect = { 485, 829, 328, 103 };
	//hoverButton = { 0, 113, 229, 69 };
	//clickButton = { 411, 169, 229, 69 };
	//normalButton = { 642, 169, 229, 69 };

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

//---------------------------------------------------------------------

	////Background
	////App->gui->PrintBackground(background, 0, 0);
	////App->gui->PrintBackground(upper, 0, 0); 

	////Gui ---
	////Top image
	//App->MGui->PrintIcon(App->MGui->GetAtlas(), true, 0, 25, 620, 5, &atlasRect, "Hello", { 255, 140, 0, 100 });

	//App->MGui->PrintIcon(App->MGui->GetAtlas(), true, 0, 250, 620, 250, &normalButton, "Dark", { 255, 140, 0, 100 });
	//App->MGui->PrintIcon(App->MGui->GetAtlas(), true, 0, 450, 620, 450, &clickButton, "Bright", { 255, 140, 0, 100 });
	//App->MGui->PrintIcon(App->MGui->GetAtlas(), true, 0, 650, 620, 650, &hoverButton, "Normal", { 255, 140, 0, 100 });

	////Wow
	////App->gui->PrintIcon(greyButton, false, 585, 345, 600, 345, NULL, "Account Name", { 255, 140, 0, 100 });
	////App->gui->PrintIcon(greyButton, false, 585, 435, 590, 435, NULL, "Account Password", { 255, 140, 0, 100 });
	////App->gui->PrintIcon(redButton, false, 585, 495, 635, 495, NULL, "Login", { 255, 140, 0, 100 });

	//if (App->MInput->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	//{
	//	App->MGui->buttonChange = true;
	//}
	//else if (App->MInput->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	//{
	//	App->MGui->buttonChange = false;
	//}

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
		/*ret = false;*/
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