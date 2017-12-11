#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;
class GuiImage;
class GuiText;
struct _TTF_Font;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

public:
	SDL_Texture* background_text = nullptr;
	SDL_Rect background_rect;

	bool lost = false;

//---------------------------------------------------------------------
private:
	SDL_Texture* debug_tex = nullptr;
	GuiImage* banner;
	GuiText* text;

	SDL_Rect atlasRect;
	SDL_Rect hoverButton;
	SDL_Rect clickButton;
	SDL_Rect normalButton;

	SDL_Texture* background = nullptr;
	SDL_Texture* upper = nullptr;

	//Wow
	SDL_Texture* greyButtonWOW = nullptr;
	SDL_Texture* redButtonWOW = nullptr;
};

#endif // __j1SCENE_H__