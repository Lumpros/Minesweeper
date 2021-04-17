#pragma once

#include <SDL_ttf.h>

#include <string>

namespace Game
{
	/* Used to make allocation easier */
	class Font
	{
	private:
		TTF_Font* mFont = nullptr;

	public:
		Font(const Font&) = delete;

		Font(const char* path, int font_size) noexcept;
		~Font(void) noexcept;

		TTF_Font* GetFont(void) const noexcept;
	};


	class Text
	{
	private:
		Font* mFont;
		SDL_Texture* mTexture = nullptr;
		const char* mText;
		SDL_Color text_color;

		void AssertSurfacePointer(SDL_Surface*);
		void AssertTexturePointer(SDL_Texture*);
		SDL_Texture* GetTextureFromLoadedSurface(SDL_Surface*);

	public:
		Text(Font* font, const char*, SDL_Color text_color) noexcept;
		~Text(void) noexcept;

		void Free(void) noexcept;

		void SetText(const char* text) noexcept;
		void SetColor(SDL_Color color) noexcept;

		/* Creates a texture using a font, text, and color 
		   texture is used in Render() */
		bool LoadFromRenderedText(const char* path, SDL_Color text_color) noexcept;

		void Render(SDL_Rect renderQuad, SDL_Rect* clip = nullptr) noexcept;
	};
}
