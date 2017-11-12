//#ifndef __ENEMY_H__
//#define __ENEMY_H__
//
//#include "p2Point.h"
//#include "Animation.h"
//
//struct SDL_Texture;
//struct Collider;
//
//class Enemy
//{
//protected:
//	Animation* animation = nullptr;
//	Collider* collider = nullptr;
//
//	Animation e1_forward;
//	Animation e1_backward;
//	Animation e1_left;
//	Animation e1_right;
//	Animation e1_up_right;
//	Animation e1_up_left;
//	Animation e1_down_right;
//	Animation e1_down_left;
//	Animation death;
//	Animation jump;
//	Animation walk;
//	Animation throwing;
//	iPoint initial_position;
//
//public:
//	iPoint position;
//	iPoint prev_position;
//	int initial_angle;
//	int sub_type;
//	bool dead = false;
//	bool collision = false;
//	bool dying = false;
//	bool isglobal;
//	int direction;
//	iPoint shooting_position;
//	AnimationFrame current_frame;
//
//public:
//	Enemy(int x, int y, int angle, int sub_type, int isglobal);
//	virtual ~Enemy();
//
//	Collider* GetCollider() const;
//
//	virtual void Move() {};
//	virtual void Draw(SDL_Texture* sprites);
//	virtual void OnCollision(Collider* collider);
//	virtual int Enemy::Collisionangle(int angle);
//	Animation* GetAnimationForDirection(int dir);
//
//};
//
//#endif // __ENEMY_H__