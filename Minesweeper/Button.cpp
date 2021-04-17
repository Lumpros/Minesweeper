#include "Button.h"
#include "Resources.h"

#include <Windows.h>

void Game::Button::Update(void) noexcept
{
	SDL_Point mouse_pos;
	bool is_mouse_clicked = SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y) & SDL_BUTTON_LMASK;

	if (SDL_PointInRect(&mouse_pos, &button_rectangle))
	{
		SetCursor(LoadCursor(NULL, IDC_HAND));

		/* Mouse button is held down, the button changes size, but nothing else happens */
		if (is_mouse_clicked && !has_been_clicked) {
			has_been_clicked = true;
			AdjustButtonSize(true);
		}

		/* Mouse button is let go */
		else if (!is_mouse_clicked && has_been_clicked) {
			AdjustButtonSize(false);
			is_clicked = true;
			has_been_clicked = false;
		}

		/* This happens the iteration after the mouse is let go, in order to disable clicking too many times*/
		else
			is_clicked = false;

	}

	else
	{
		is_clicked = false;
		has_been_clicked = false;

		AdjustButtonSize(false);
	}
}

bool Game::Button::IsHeldDown(void) noexcept
{
	return has_been_clicked;
}

bool Game::Button::IsClicked(void) noexcept
{
	return is_clicked;
}

void Game::Button::Render(void) noexcept
{
	mTexture->Render(button_rectangle, &clip);
}

void Game::Button::AdjustButtonSize(bool mob) noexcept
{
	if (mob) {
		button_rectangle = {
			original_position.x - static_cast<int>((scale - 1) * original_position.w) / 2,
			original_position.y - static_cast<int>((scale - 1) * original_position.h) / 2,
			static_cast<int>(original_position.w * scale),
			static_cast<int>(original_position.h * scale)
		};
	}

	else
		button_rectangle = original_position;
}

Game::Button::Button(SDL_Rect clip, SDL_Rect button_rect, float scale) noexcept
{
	button_rectangle = (original_position = button_rect);
	mTexture = Resources::Get()->gTextures.Get("button");

	this->clip = clip;
	this->scale = scale;
}

void Game::Button::SetClip(SDL_Rect new_clip)
{
	clip = new_clip;
}