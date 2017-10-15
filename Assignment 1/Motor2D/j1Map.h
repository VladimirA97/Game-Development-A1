#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1Colliders.h"
#include "SDL/include/SDL.h"

struct map_layer
{
	p2SString name;
	uint* tiles = nullptr;
	uint size = 0;
	uint width = 0;
	uint height = 0;

	bool movement_layer = false;
	float map_scroll = 0;

	inline uint Get(int x, int y)const
	{
		return y*width + x;
	};

	~map_layer()
	{
		RELEASE_ARRAY(tiles);
	}
};

struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
};

enum MapTypes
{
	UNKNOWN_MAP = 0,
	ORTHOGONAL_MAP,
	ISOMETRIC_MAP,
	STAGGERED_MAP
};

struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	//Add a list of layers to the map
	p2List<map_layer*> layers;
	Collider* colliders[MAX_COLLIDERS];
};

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

	//A method that translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;

private:
	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, map_layer* layer);

	bool CreateColliders(map_layer* layer);

public:
	MapData data;
	p2List<p2SString>  maps;
	p2SString curr_name_map;
	uint id_map = 0;

	void switch_map(uint index);

private:
	pugi::xml_document	map_file;
	p2SString			path;
	bool				map_loaded;
};
#endif