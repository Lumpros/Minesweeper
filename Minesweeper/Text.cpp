#include "Text.h"
#include "Texture.h"
#include "Renderer.h"
#include "Exception.h"

Game::Font::Font(const char* path, int font_size) noexcept
{
	mFont = TTF_OpenFont(path, font_size);
}

Game::Font::~Font(void) noexcept
{
	 TTF_CloseFont(mFont);
}

TTF_Font* Game::Font::GetFont(void) const noexcept
{
	return mFont;
}

Game::Text::Text(Font* font, const char* text, SDL_Color text_color) noexcept
	: mFont(font), mText(text)
{
	this->text_color = text_color;
	LoadFromRenderedText(text, text_color);
}

Game::Text::~Text(void) noexcept
{
    Free();
}

void Game::Text::Free(void) noexcept
{
    if (mTexture)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
        mText = "";
    }
}

void Game::Text::SetText(const char* text) noexcept
{
    mText = text;
    LoadFromRenderedText(text, text_color);
}

void Game::Text::SetColor(SDL_Color color) noexcept
{
    text_color = color;
    LoadFromRenderedText(mText, color);
}

bool Game::Text::LoadFromRenderedText(const char* text, SDL_Color text_color) noexcept
{
    Free();
    SDL_Surface* textSurface = TTF_RenderText_Solid(mFont->GetFont(), text, text_color);

    AssertSurfacePointer(textSurface);
    mTexture = GetTextureFromLoadedSurface(textSurface);

    SDL_FreeSurface(textSurface);
    return mTexture != nullptr;
}

void Game::Text::AssertSurfacePointer(SDL_Surface* pSurface)
{
    if (pSurface == NULL)
        ThrowErrorUsingSDLMessage("Unable to render text surface! ", TTF_GetError());
}

SDL_Texture* Game::Text::GetTextureFromLoadedSurface(SDL_Surface* pSurface)
{
    SDL_Texture* pTexture = SDL_CreateTextureFromSurface(gRenderer, pSurface);
    AssertTexturePointer(pTexture);
    return pTexture;
}

void Game::Text::AssertTexturePointer(SDL_Texture* pTexture)
{
    if (pTexture == NULL)
        ThrowErrorUsingSDLMessage("Unable to create texture from rendered text! ", SDL_GetError());
}

void Game::Text::Render(SDL_Rect renderQuad, SDL_Rect* clip) noexcept
{
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}
