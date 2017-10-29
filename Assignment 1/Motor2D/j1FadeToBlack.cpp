#include <iostream>
#include <math.h>
#include "j1App.h"
#include "j1FadeToBlack.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"

j1FadeToBlack::j1FadeToBlack()
{
	/*screen = { 0, 0, SCREEN_WIDTH * SCREEN_SIZE, SCREEN_HEIGHT * SCREEN_SIZE };*/
	screen = { 0, 0, 1024 * (1024*700), 700 * (1024 * 700) };
}

j1FadeToBlack::~j1FadeToBlack()
{}

// Load assets
bool j1FadeToBlack::Start()
{
	//LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->MRender->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

// Update: draw background
update_status j1FadeToBlack::Update()
{
	if (current_step == fade_step::none)
		return UPDATE_CONTINUE;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
	case fade_step::fade_to_black:
	{
		if (now >= total_time)
		{
			this->module_off->Disable();
			this->module_on->Enable();

			if (App->MScene->lost)
				App->MScene->lost = false;

			total_time += total_time;
			start_time = SDL_GetTicks();
			current_step = fade_step::fade_from_black;
		}
	} break;

	case fade_step::fade_from_black:
	{
		normalized = 1.0f - normalized;

		if (now >= total_time)
			current_step = fade_step::none;
	} break;
	}

	// Finally render the black square with alpha on the screen
	SDL_SetRenderDrawColor(App->MRender->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->MRender->renderer, &screen);

	return UPDATE_CONTINUE;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool j1FadeToBlack::FadeToBlack(j1Module* module_off, j1Module* module_on, float time)
{
	bool ret = false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		ret = true;
	}
	this->module_off = module_off;
	this->module_on = module_on;

	return ret;
}

bool j1FadeToBlack::GetFadeState()
{
	bool ret = false;
	if (current_step != fade_step::none)
	{
		ret = true;
	}

	return ret;
}