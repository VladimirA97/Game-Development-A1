#ifndef __ModuleFonts_H__
#define __ModuleFonts_H__

#include "j1Module.h"
#include "SDL\include\SDL_pixels.h"
#include "j1Window.h"

#define MAX_FONTS 60
#define MAX_FONT_CHARS 256

struct SDL_Texture;

//struct Font
//{
//	char table[MAX_FONT_CHARS];
//	SDL_Texture* graphic = nullptr;
//	uint rows, len, char_w, char_h, row_chars;
//	uint spacing, margin;
//	const char* path;
//};
//
//class ModuleFonts : public Module
//{
//public:
//
//	ModuleFonts();
//	~ModuleFonts();
//
//	// Load Font
//	int Load(const char* texture_path, const char* characters, uint rows = 1, uint margin = 0, uint spacing = 0);
//	void UnLoad(int font_id);
//	iPoint getFontDimensions(int id);
//	bool CleanUp();
//	bool Init();
//
//	// Create a surface from text
//	void BlitText(Label* label) const;
//	int LoadWhiteFont();
//
//private:
//	Font fonts[MAX_FONTS];
//};
//
//
//#endif // __ModuleFonts_H__
