#include "j1Fonts.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"

//#include <string.h>

// Constructor
//ModuleFonts::ModuleFonts() : Module()
//{
//}
//
//// Destructor
//ModuleFonts::~ModuleFonts()
//{}
//
//bool ModuleFonts::Init() {
//	return true;
//}
//
//// Load new texture from file path
//int ModuleFonts::Load(const char* texture_path, const char* characters, uint rows, uint margin, uint spacing)
//{
//	int id = -1;
//	const uint char_amount = strlen(characters);
//	if (texture_path == nullptr || characters == nullptr || rows == 0)
//	{
//		LOG("Could not load font");
//		return id;
//	}
//
//	for (int i = 0; i < MAX_FONTS; i++)
//		if (fonts[i].path == texture_path)
//			return i;
//
//	SDL_Texture* tex = App->textures->Load(texture_path);
//
//	if (tex == nullptr || char_amount >= MAX_FONT_CHARS)
//	{
//		LOG("Could not load font at %s with characters '%s'", texture_path, characters);
//		return id;
//	}
//
//	id = 0;
//	for (; id < MAX_FONTS; ++id)
//		if (fonts[id].graphic == nullptr)
//			break;
//
//	if (id == MAX_FONTS)
//	{
//		LOG("Cannot load font %s. Array is full (max %d).", texture_path, MAX_FONTS);
//		return id;
//	}
//
//	fonts[id].path = texture_path;
//	fonts[id].graphic = tex; // graphic: pointer to the texture
//	fonts[id].rows = rows; // rows: rows of characters in the texture
//	fonts[id].len = char_amount; // len: length of the table
//	fonts[id].margin = margin;
//	fonts[id].spacing = spacing;
//	// TODO 1: Finish storing font data
//
//	uint texture_width = 0, texture_height = 0;
//	for (int i = 0; i < fonts[id].len; i++) {
//		fonts[id].table[i] = characters[i];
//	}
//	fonts[id].row_chars = fonts[id].len / fonts[id].rows;
//	App->textures->GetSize(tex, texture_width, texture_height);
//	fonts[id].char_w = (texture_width - (fonts[id].margin * 2) - (fonts[id].spacing * (fonts[id].row_chars - 1))) / fonts[id].row_chars;
//	fonts[id].char_h = (texture_height - (fonts[id].margin * 2) - (fonts[id].spacing * (fonts[id].rows - 1))) / fonts[id].rows;
//
//	// table: array of chars to have the list of characters
//	// row_chars: amount of chars per row of the texture
//	// char_w: width of each character
//	// char_h: height of each character
//
//	LOG("Successfully loaded BMP font from %s", texture_path);
//
//	return id;
//}
//
//void ModuleFonts::UnLoad(int font_id)
//{
//	if (font_id >= 0 && font_id < MAX_FONTS && fonts[font_id].graphic != nullptr)
//	{
//		App->textures->Unload(fonts[font_id].graphic);
//		fonts[font_id].graphic = nullptr;
//		fonts[font_id].path = "";
//		LOG("Successfully Unloaded BMP font_id %d", font_id);
//	}
//}
//
//// Render text using a bitmap font
//void ModuleFonts::BlitText(Label* label) const
//{
//	if (label->string == nullptr || label->font_id < 0 || label->font_id >= MAX_FONTS || fonts[label->font_id].graphic == nullptr)
//	{
//		LOG("Unable to render text with bmp font id %d", label->font_id);
//		return;
//	}
//
//	const Font* font = &fonts[label->font_id];
//	SDL_Rect rect;
//	uint row = 0;
//	uint col = 0;
//	const uint len = strlen(label->string);
//	uint total_rows = 1;
//
//	rect.w = font->char_w;
//	rect.h = font->char_h;
//
//	for (uint i = 0; label->string[i] != '\0'; i++) {
//		switch (label->string[i]) {
//		case '\n':
//			total_rows++;
//			break;
//		}
//	}
//
//	uint *row_len = (uint*)SDL_calloc(total_rows, sizeof(uint));
//
//	{	//Enclosed Scope
//		uint char_count = 0;
//		for (uint i = 0; label->string[i] != '\0'; i++) {
//			if (label->string[i] == '\n') {
//				row_len[row] = char_count;
//				char_count = 0;
//				row++;
//			}
//			else char_count++;
//		}
//		row_len[row] = char_count;
//		row = 0;
//	}
//
//	for (uint i = 0; i < len; ++i, col++)
//	{
//		if (label->string[i] == '\n') {
//			row++;
//			col = -1;
//			continue;
//		}
//		// TODO 2: Find the character in the table and its position in the texture, then Blit
//		for (uint j = 0; j < font->len; j++) {
//			if (font->table[j] == label->string[i]) {
//				rect.y = (j / font->row_chars);
//				rect.x = (j - rect.y * font->row_chars);
//				rect.y *= font->char_h;
//				rect.y += font->margin + font->spacing * (rect.y / font->char_h);
//				rect.x *= font->char_w;
//				rect.x += font->margin + font->spacing * (rect.x / font->char_w);
//				break;
//			}
//		}
//		int posX = 0;
//		switch (label->alignment)
//		{
//		case ALIGNMENT_LEFT:
//			posX = label->pos.x + col * rect.w;
//			break;
//		case ALIGNMENT_RIGHT:
//			posX = label->pos.x - (font->char_w * row_len[row]) + col * rect.w;
//			break;
//		case ALIGNMENT_CENTRE:
//			posX = label->pos.x - ((font->char_w * row_len[row]) / 2) + col * rect.w;
//			break;
//		default:
//			posX = label->pos.x + col * rect.w;
//			break;
//		}
//		App->render->Blit(font->graphic, posX, label->pos.y + row * (rect.h + 1), &rect, 0.0f, false);
//	}
//}
//
//int ModuleFonts::LoadWhiteFont() {
//	return App->fonts->Load("Images/Fuentes_small_grey.png",
//		"0123456789ABCDEF\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1           K;®.,0123456789=      ABCDEFGHIJKLMNOPQRSTUVWXYZ.\1\1   abcdefghijklmnopqrstuvwxyz    |                                ",
//		5, 0, 1);
//}
//
//iPoint ModuleFonts::getFontDimensions(int id) {
//	iPoint dimensions;
//	dimensions.x = App->fonts->fonts[id].char_w;
//	dimensions.y = App->fonts->fonts[id].char_h;
//	return dimensions;
//}
//
//bool ModuleFonts::CleanUp() {
//	for (uint i = 0; i < MAX_FONTS; i++) {
//		UnLoad(i);
//	}
//	return true;
//}