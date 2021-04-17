#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>

#include "Preprocessors.h"

namespace Game
{

	class Texture
	{
	private:
		SDL_Texture* mTexture = nullptr;
		int mWidth, mHeight;

		void AssertLoadedSurface(SDL_Surface*, const std::string&);
		void AssertTexturePointer(SDL_Texture*, const std::string&);
		SDL_Texture* GetTextureFromLoadedSurface(SDL_Surface*, const std::string&);

	public:
		explicit Texture(const char* path) noexcept;
		Texture(void) noexcept;
		~Texture(void) noexcept;

		bool NODISCARD LoadFromFile(const char* path) noexcept;
	
		void Free(void) noexcept;
		
		void Render(SDL_Rect renderQuad, SDL_Rect* clip = nullptr) noexcept;
	};
}
