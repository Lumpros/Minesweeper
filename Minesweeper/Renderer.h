#pragma once

#include <SDL.h>
#include <vector>

#include "Entity.h"

namespace Game
{
	extern SDL_Renderer* gRenderer;

	typedef std::vector<Renderable*> RenderContainer;

	class Renderer
	{
	private:
		RenderContainer submitted_to_render;
		SDL_Color clear_color = { 0x00, 0x00, 0x00, 0xFF };
		signed int render_container_size = 0;

		Renderer(void) = default;

		void InsertRenderableToAppropriatePosition(Renderable*);
		signed int CalculatePositionInOrderToBeSorted(Renderable*);

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