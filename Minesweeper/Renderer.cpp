#include "Renderer.h"

bool Game::RenderableComparison(Game::Renderable* a, Game::Renderable* b)
{
	return a->z_index > b->z_index;
}

void Game::Renderer::SubmitToRender(Renderable* pRenderable)
{
	submitted_to_render.push(pRenderable);
	++render_container_size;
}

void Game::Renderer::RenderSubmittedItems(void)
{
	SDL_SetRenderDrawColor(gRenderer, clear_color.r, clear_color.g, clear_color.b, 0xFF);
	SDL_RenderClear(gRenderer);

	for (signed int i = 0; i < render_container_size; ++i)
	{
		submitted_to_render.top()->Render();
		submitted_to_render.pop();
	}
	render_container_size = 0;

	SDL_RenderPresent(gRenderer);
}

Game::Renderer* Game::Renderer::Get(void)
{
	static Renderer instance;
	return &instance;
}

void Game::Renderer::SetClearColor(SDL_Color newColor)
{
	clear_color = newColor;
}