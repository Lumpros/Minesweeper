#include "Renderer.h"

void Game::Renderer::SubmitToRender(Renderable* pRenderable)
{
	InsertRenderableToAppropriatePosition(pRenderable);
	++render_container_size;
}

void Game::Renderer::InsertRenderableToAppropriatePosition(Renderable* pRenderable)
{
	signed int position = CalculatePositionInOrderToBeSorted(pRenderable);
	submitted_to_render.insert(submitted_to_render.begin() + position, pRenderable);
}

signed int Game::Renderer::CalculatePositionInOrderToBeSorted(Renderable* pRenderable)
{
	signed int i = 0;
	for (; i < render_container_size - 1; ++i) {
		short current_z_index = submitted_to_render[i]->z_index;
		short next_z_index = submitted_to_render[i + 1]->z_index;
		if (current_z_index < next_z_index)
			break;
	}
	return i;
}

void Game::Renderer::RenderSubmittedItems(void)
{
	SDL_SetRenderDrawColor(gRenderer, clear_color.r, clear_color.g, clear_color.b, 0xFF);
	SDL_RenderClear(gRenderer);

	for (signed int i = 0; i < render_container_size; ++i)
		submitted_to_render[i]->Render();
	render_container_size = 0;
	submitted_to_render.clear();

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