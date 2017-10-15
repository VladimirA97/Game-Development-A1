#ifndef __J1COLLIDERS_H__
#define __J1COLLIDERS_H__

#define MAX_COLLIDERS 500

#include "j1Module.h"
#include "SDL/include/SDL.h"

enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,

	COLLIDER_PLAYER,
	COLLIDER_WALL,
	COLLIDER_WATER,

	COLLIDER_PREVENT,

	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	COLLIDER_TYPE clld_type;
	j1Module* callback = nullptr;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, j1Module* callback = nullptr) :
		rect(rectangle),
		clld_type(type),
		callback(callback)
	{}

	void SetPosition(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;
};

class j1Colliders :	public j1Module
{
public:
	j1Colliders();
	~j1Colliders();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	bool Awake(pugi::xml_node&);

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback = nullptr);
	void DebugDraw();

private:

	Collider* colliders[MAX_COLLIDERS];
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	bool debug = true;
};

#endif // __ModuleCollision_H__
