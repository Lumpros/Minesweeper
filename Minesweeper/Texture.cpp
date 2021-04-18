#include "Texture.h"
#include "Exception.h"
#include "Renderer.h"

using FailedToLoadImageException = Exception;

Game::Texture::Texture(void) noexcept
{
	mWidth = mHeight = 0;
}

Game::Texture::Texture(const char* path)
{
	if (!LoadFromFile(path)) {
		const std::string message = "Failed to load ";
		throw FailedToLoadImageException(message + path);
	}
}

Game::Texture::~Texture(void)
{
	Free();
}

bool NODISCARD Game::Texture::LoadFromFile(const char* path) noexcept
{
	Free();

	SDL_Surface* loaded_surface = IMG_Load(path);
	AssertLoadedSurface(loaded_surface, path);
	mTexture = GetTextureFromLoadedSurface(loaded_surface, path);

	SDL_FreeSurface(loaded_surface);
	return mTexture != NULL;
}

void Game::Texture::AssertLoadedSurface(SDL_Surface* pSurface, const std::string& img_path)
{
	if (pSurface == NULL)
		ThrowErrorUsingSDLMessage("Unable to load image " + img_path, IMG_GetError());
}

SDL_Texture* Game::Texture::GetTextureFromLoadedSurface(SDL_Surface* loaded_surface, const std::string& img_path)
{
	SDL_Texture* new_texture = SDL_CreateTextureFromSurface(gRenderer, loaded_surface);
	AssertTexturePointer(new_texture, img_path);
	mWidth = loaded_surface->w;
	mHeight = loaded_surface->h;
	return new_texture;
}

void Game::Texture::AssertTexturePointer(SDL_Texture* pTexture, const std::string& img_path)
{
	if (!pTexture)
		ThrowErrorUsingSDLMessage("Unable to create texture from " + img_path, SDL_GetError());
}

void Game::Texture::Free(void) noexcept
{
	if (mTexture)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = nullptr;
		mWidth = mHeight = 0;
	}
}

void Game::Texture::Render(SDL_Rect renderQuad, SDL_Rect* clip) noexcept
{
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}