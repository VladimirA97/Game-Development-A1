#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 200

class Animation
{
public:
	bool loop = true;
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];

private:
	float current_frame = 0.0f;
	int last_frame = 0;
	int loops = 0;

public:

	Animation()
	{}

	Animation(const Animation& anim) : loop(anim.loop), speed(anim.speed), last_frame(anim.last_frame)
	{
		SDL_memcpy(&frames, anim.frames, sizeof(frames));
	}

	void PushBack(const SDL_Rect& rect)
	{
		if (last_frame <= MAX_FRAMES)
			frames[last_frame++] = rect;
	}

	SDL_Rect& GetCurrentFrame()
	{
		current_frame += speed;
		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : last_frame - 1;
			loops++;
		}

		return frames[(int)current_frame];
	}

	SDL_Rect& GetRect(int frame)
	{
		return frames[frame];
	}

	SDL_Rect& GetCurrentRect()
	{
		return frames[(int)current_frame];
	}

	bool Finished() const
	{
		return loops > 0;
	}

	int GetCurrentFrameNum() const
	{
		return current_frame / speed;
	}

	void Reset()
	{
		loops = 0;
		current_frame = 0.0f;
	}
};

#endif
