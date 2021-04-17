#include "Resources.h"

Game::Resources* Game::Resources::Get(void) noexcept
{
	static Resources instance;
	return &instance;
}

void Game::Resources::ReleaseResources(void) 
{
	gTextures.resources.clear();
	gFonts.resources.clear();
	gSFX.resources.clear();
	gMusic.resources.clear();
}

Game::Resources::Resources(void) noexcept
{
}
