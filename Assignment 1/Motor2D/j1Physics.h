//#ifndef __J1PHYSICS_H__
//#define __J1PHYSICS_H__
//
//#include "j1Module.h"
//#include "p2Point.h"
//#include "j1Collision.h"
//
//#define MAX_OBJECTS 15
//
//struct object
//{
//	fPoint position;
//	fPoint velocity;
//	fPoint acceleration;
//	Collider* col;
//	Collider* predictor;
//	bool grounded = false;
//};
//
//class j1Physics : public j1Module
//{
//public:
//	j1Physics();
//	~j1Physics() {};
//
//	bool PreUpdate();
//	bool Update(float dt);
//	bool PostUpdate();
//
//	bool Awake(pugi::xml_node* config);
//
//	object* Addobject(float x, float y, float gravity, SDL_Rect* collision, COLLIDER_TYPE TYPE, j1Module* callback = NULL);
//
//	void OnCollision(Collider* c1, Collider* c2);
//
//	bool CleanUp();
//
//	object* objects[MAX_OBJECTS];
//
//	object* GetObjectFromRect_predictor(SDL_Rect* rectangle);
//};
//#endif // !__J1PHYSICS_H__
//
