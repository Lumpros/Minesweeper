#include "Sound.h"

#include <cstdio>

Game::Music::Music(const char* music_path) noexcept
{
	LoadFromFile(music_path);
}

Game::Music::~Music(void) noexcept
{
	Mix_FreeMusic(m_Music);
}

bool Game::Music::LoadFromFile(const char* music_path) noexcept
{
	m_Music = Mix_LoadMUS(music_path);

	if (m_Music == NULL)
		printf("Failed to load %s music! SDL_mixer Error: %s\n", music_path, Mix_GetError());

	return m_Music != NULL;
}

void Game::Music::Play(int loop)
{
	if (!Music::IsMusicPlaying())
		Mix_PlayMusic(m_Music, loop);
}

Game::SFX::SFX(const char* sfx_path) noexcept
{
	LoadFromFile(sfx_path);
}

Game::SFX::~SFX(void) noexcept
{
	Mix_FreeChunk(m_Chunk);
}

bool Game::SFX::LoadFromFile(const char* sfx_path) noexcept
{
	m_Chunk = Mix_LoadWAV(sfx_path);

	return m_Chunk != NULL;
}

void Game::SFX::Play(void)
{
	Mix_PlayChannel(-1, m_Chunk, 0);
}
