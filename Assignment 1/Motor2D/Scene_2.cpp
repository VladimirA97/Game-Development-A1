//#include "p2Defs.h"
//#include "p2Log.h"
//#include "j1App.h"
//#include "j1Input.h"
//#include "j1Textures.h"
//#include "j1Audio.h"
//#include "j1Render.h"
//#include "j1Window.h"
//#include "j1Map.h"
//#include "j1Scene.h"
//#include "j1Player.h"
//#include "j1Movement.h"
//
//j1Scene::j1Scene() : j1Module()
//{
//	name.create("Test");
//	background_rect.w = 3000;
//	background_rect.h = 750;
//}
//
//// Destructor
//j1Scene::~j1Scene()
//{}
//
//// Called before render is available
//bool j1Scene::Awake()
//{
//	LOG("Loading Scene");
//	bool ret = true;
//
//	return ret;
//}
//
//// Called before the first frame
//bool j1Scene::Start()
//{
//	App->MMap->Load(App->MMap->curr_name_map.GetString());
//	//App->MMap->Load("Map 1.tmx");
//
//	background_text = App->MTextures->Load("maps/BG1.png");
//
//	App->MAudio->PlayMusic("audio/music/Adventure.ogg");
//
//	return true;
//}
//
//// Called each loop iteration
//bool j1Scene::PreUpdate()
//{
//	return true;
//}
//
//// Called each loop iteration
//bool j1Scene::Update(float dt)
//{
//	uint win;
//	uint i;
//	App->MWindow->GetWindowSize(win, i);
//	App->MRender->camera.x = -App->MPlayer->player->position.x + win / 2;
//
//	if (App->MRender->camera.x < win / 2)
//	{
//		App->MRender->camera.x = 0;
//	}
//
//	if (-App->MRender->camera.x >((App->MMap->data.width*App->MMap->data.tile_width) - win))
//	{
//		App->MRender->camera.x = -(App->MMap->data.width*App->MMap->data.tile_width) + win;
//	}
//
//	if (App->MInput->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
//		App->SaveGame();
//
//	if (App->MInput->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
//		App->LoadGame();
//
//	//if (App->MInput->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
//	//	App->MRender->camera.y += 1;
//
//	//if (App->MInput->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
//	//	App->MRender->camera.y -= 1;
//
//	App->MRender->Blit(background_text, App->MRender->camera.x - 500, App->MRender->camera.y, &background_rect, 0.01f);
//	App->MMap->Draw();
//
//	p2SString title("Escape");
//	App->MWindow->SetTitle(title.GetString());
//
//	return true;
//}
//
//// Called each loop iteration
//bool j1Scene::PostUpdate()
//{
//	bool ret = true;
//
//	if (App->MInput->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
//		ret = false;
//
//	return ret;
//}
//
//// Called before quitting
//bool j1Scene::CleanUp()
//{
//	LOG("Freeing scene");
//
//	if (background_text != nullptr)
//	{
//		App->MTextures->UnLoad(background_text);
//		background_text = nullptr;
//	}
//
//	return true;
//}
