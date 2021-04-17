#pragma once

#include <SDL.h>

namespace Game
{
	class Renderable
	{
	public:
		SDL_Rect hitbox = { 0, 0, 0, 0 };
		short z_index = 0;

		virtual void Render(void) = 0;
	};

	class Entity : public Renderable
	{
	public:
		virtual void Update(void) = 0;
	};
}