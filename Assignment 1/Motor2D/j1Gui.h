#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "j1Fonts.h"

#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions
	bool PrintBackground(SDL_Texture* texture, int xPos, int yPos);
	bool j1Gui::PrintIcon(SDL_Texture* texture, bool center, int xPos, int yPos, int txPos, int tyPos, SDL_Rect* rect, const char* string, SDL_Color colour);
	bool j1Gui::IsMouseOverButton(SDL_Rect* rect);

	SDL_Texture* GetAtlas(); //const

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;

	_TTF_Font* font;

public:
	bool buttonChange = false;
};

#endif // __j1GUI_H__