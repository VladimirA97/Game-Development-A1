#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Window.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->MTextures->Load(atlas_file_name.GetString());

	font = App->MFont->Load("fonts/open_sans/OpenSans-Light.ttf", 14);

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	return true;
}

// const getter for atlas
SDL_Texture* j1Gui::GetAtlas()
{
	return atlas;
}

// class Gui ---------------------------------------------------
bool j1Gui::PrintBackground(SDL_Texture* texture, int xPos, int yPos)
{
	bool ret = true;

	App->MRender->Blit(texture, xPos, yPos, NULL, 0.0f);

	return ret;
}

bool j1Gui::PrintIcon(SDL_Texture* texture, bool center, int xPos, int yPos, int txPos, int tyPos, SDL_Rect* rect, const char* string, SDL_Color colour)
{
	bool ret = true;

	if (center == true)
	{
		//xPos = (SCREEN_WIDTH / 2) - (rect->w / 2);
		xPos = (1024 / 2) - (rect->w / 2);
		txPos = (1024 / 2);
	}

	//App->Colliders->DrawQuad(rect);
	App->MRender->Blit(texture, xPos, yPos, rect, 0.0f);
	App->MRender->Blit(App->MFont->Print(string, colour, font), txPos, tyPos, NULL, 0.0f);

	return ret;
}

bool j1Gui::IsMouseOverButton(SDL_Rect* rect)
{
	bool ret = true;

	//int x, y;
	App->MInput->GetMousePosition(rect->x, rect->y);

	return ret;
}