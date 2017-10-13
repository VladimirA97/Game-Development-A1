#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	// TODO 5: Prepare the loop to draw all tilesets + Blit
	p2List_item<MapLayer*>* item = nullptr;
	MapLayer* layer = nullptr;
	uint tile_id;
	p2List_item<TileSet*>* tileSet = nullptr;
	for (item = data.MapDa_layers.start; item; item = item->next)
	{
		layer = item->data;
		
		for (int j = 0; j < data.MapDa_height; j++)
		{
			for (int i = 0; i < data.MapDa_width; i++)
			{
				tile_id = layer->MapLay_tiles[layer->MapLay_Get(i, j)];
				if (tile_id != 0)
				{
					for (tileSet = data.MapDa_tilesets.start; tileSet; tileSet = tileSet->next)
					{
						if (tile_id >= tileSet->data->TileSet_firstgid && ((!tileSet->next) || (tileSet->next && tile_id < tileSet->next->data->TileSet_firstgid)))
						{
							break;
						}
					}
					
					SDL_Texture* texture = tileSet->data->TileSet_texture;
					iPoint position = MapToWorld(i, j);
					SDL_Rect* section = &tileSet->data->TileSet_GetTileRect(tile_id);
					App->render->Blit(texture, position.x, position.y, section);
				}
			}
		}
	}

		// TODO 9: Complete the draw function

}


iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * data.MapDa_tile_width;
	ret.y = y * data.MapDa_tile_height;

	return ret;
}

SDL_Rect TileSet::TileSet_GetTileRect(int id) const
{
	int relative_id = id - TileSet_firstgid;
	SDL_Rect rect;
	rect.w = TileSet_tile_width;
	rect.h = TileSet_tile_height;
	rect.x = TileSet_margin + ((rect.w + TileSet_spacing) * (relative_id % TileSet_num_tiles_width));
	rect.y = TileSet_margin + ((rect.h + TileSet_spacing) * (relative_id / TileSet_num_tiles_width));
	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.MapDa_tilesets.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.MapDa_tilesets.clear();

	// TODO 2: clean up all layer data
	// Remove all layers
	p2List_item<MapLayer*>* item_layer;
	item_layer = data.MapDa_layers.start;

	while (item_layer != NULL)
	{
		RELEASE(item_layer->data);
		item_layer = item_layer->next;
	}
	data.MapDa_layers.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.MapDa_tilesets.add(set);
	}

	// TODO 4: Iterate all layers and load each of them
	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for (layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* set = new MapLayer();

		if (ret == true)
		{
			ret = LoadLayer(layer, set);
		}

		data.MapDa_layers.add(set);
	}

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.MapDa_width, data.MapDa_height);
		LOG("tile_width: %d tile_height: %d", data.MapDa_tile_width, data.MapDa_tile_height);

		p2List_item<TileSet*>* item = data.MapDa_tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->TileSet_name.GetString(), s->TileSet_firstgid);
			LOG("tile width: %d tile height: %d", s->TileSet_tile_width, s->TileSet_tile_height);
			LOG("spacing: %d margin: %d", s->TileSet_spacing, s->TileSet_margin);
			item = item->next;
		}

		// TODO 4: Add info here about your loaded layers
		// Adapt this vcode with your own variables
		
		p2List_item<MapLayer*>* item_layer = data.MapDa_layers.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->MapLay_name.GetString());
			LOG("tile width: %d tile height: %d", l->MapLay_width, l->MapLay_height);
			item_layer = item_layer->next;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.MapDa_width = map.attribute("width").as_int();
		data.MapDa_height = map.attribute("height").as_int();
		data.MapDa_tile_width = map.attribute("tilewidth").as_int();
		data.MapDa_tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.MapDa_background_color.r = 0;
		data.MapDa_background_color.g = 0;
		data.MapDa_background_color.b = 0;
		data.MapDa_background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.MapDa_background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.MapDa_background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.MapDa_background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.MapDa_type = ORTHOGONAL_MAP;
		}
		else if(orientation == "isometric")
		{
			data.MapDa_type = ISOMETRIC_MAP;
		}
		else if(orientation == "staggered")
		{
			data.MapDa_type = STAGGERED_MAP;
		}
		else
		{
			data.MapDa_type = UNKNOWN_MAP;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->TileSet_name.create(tileset_node.attribute("name").as_string());
	set->TileSet_firstgid = tileset_node.attribute("firstgid").as_int();
	set->TileSet_tile_width = tileset_node.attribute("tilewidth").as_int();
	set->TileSet_tile_height = tileset_node.attribute("tileheight").as_int();
	set->TileSet_margin = tileset_node.attribute("margin").as_int();
	set->TileSet_spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->TileSet_offset_x = offset.attribute("x").as_int();
		set->TileSet_offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->TileSet_offset_x = 0;
		set->TileSet_offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->TileSet_texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->TileSet_texture, NULL, NULL, &w, &h);
		set->TileSet_tex_width = image.attribute("width").as_int();

		if(set->TileSet_tex_width <= 0)
		{
			set->TileSet_tex_width = w;
		}

		set->TileSet_tex_height = image.attribute("height").as_int();

		if(set->TileSet_tex_height <= 0)
		{
			set->TileSet_tex_height = h;
		}

		set->TileSet_num_tiles_width = set->TileSet_tex_width / set->TileSet_tile_width;
		set->TileSet_num_tiles_height = set->TileSet_tex_height / set->TileSet_tile_height;
	}

	return ret;
}

// TODO 3: Create the definition for a function that loads a single layer
bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->MapLay_name = node.attribute("name").as_string();
	layer->MapLay_width = node.attribute("width").as_int();
	layer->MapLay_height = node.attribute("height").as_int();
	layer->MapLay_size = (layer->MapLay_width * layer->MapLay_height);

	layer->MapLay_tiles = new uint[layer->MapLay_size];
	memset(layer->MapLay_tiles, 0, sizeof(uint) * layer->MapLay_size);

	pugi::xml_node tile;
	int i = 0;
	for (tile = node.child("data").child("tile"); tile; tile = tile.next_sibling("tile"))
	{
		layer->MapLay_tiles[i] = tile.attribute("gid").as_uint();
		i++;
	}

	return ret;
}