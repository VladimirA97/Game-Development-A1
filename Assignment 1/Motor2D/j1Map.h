#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

// TODO 1: Create a struct for the map layer
// ----------------------------------------------------
struct MapLayer
{
	p2SString MapLay_name;
	int MapLay_width;
	int MapLay_height;
	uint* MapLay_tiles = nullptr;
	int MapLay_size;
	inline uint MapLay_Get(int x, int y) const
	{
		return (x + y * MapLay_width);
	}

	~MapLayer()
	{
		RELEASE(MapLay_tiles);
	}

};
	// TODO 6: Short function to get the value of x,y



// ----------------------------------------------------
struct TileSet
{
	// TODO 7: Create a method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
	SDL_Rect TileSet_GetTileRect(int id) const;

	p2SString			TileSet_name;
	int					TileSet_firstgid;
	int					TileSet_margin;
	int					TileSet_spacing;
	int					TileSet_tile_width;
	int					TileSet_tile_height;
	SDL_Texture*		TileSet_texture;
	int					TileSet_tex_width;
	int					TileSet_tex_height;
	int					TileSet_num_tiles_width;
	int					TileSet_num_tiles_height;
	int					TileSet_offset_x;
	int					TileSet_offset_y;
};

enum MapTypes
{
	UNKNOWN_MAP = 0,
	ORTHOGONAL_MAP,
	ISOMETRIC_MAP,
	STAGGERED_MAP
};
// ----------------------------------------------------
struct MapData
{
	int					MapDa_width;
	int					MapDa_height;
	int					MapDa_tile_width;
	int					MapDa_tile_height;
	SDL_Color			MapDa_background_color;
	MapTypes			MapDa_type;
	p2List<TileSet*>	MapDa_tilesets;
	// TODO 2: Add a list/array of layers to the map!
	p2List<MapLayer*> MapDa_layers;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	// TODO 8: Create a method that translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	// TODO 3: Create a method that loads a single laye
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);

public:

	MapData data;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__