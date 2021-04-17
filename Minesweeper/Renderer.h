#pragma once

#include <SDL.h>
#include <queue>
#include <functional>

#include "Entity.h"

namespace Game
{
	extern SDL_Renderer* gRenderer;

	bool RenderableComparison(Renderable* a, Renderable* b);

	class Renderer
	{
	private:
		SDL_Color clear_color = { 0x00, 0x00, 0x00, 0xFF };
		signed int render_container_size = 0;

		std::priority_queue <
			Renderable*,
			std::vector<Renderable*>,
			std::function<bool(Renderable*, Renderable*)>
		> submitted_to_render;

		Renderer(void) : submitted_to_render(RenderableComparison) {}

	public:
		void SubmitToRender(Renderable* pRenderable);
		void RenderSubmittedItems(void);

		void SetClearColor(SDL_Color newColor);

		static Renderer* Get(void);

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;

		void operator=(const Renderer&) = delete;
		void operator=(Renderer&&) = delete;
	};
}