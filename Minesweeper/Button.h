#pragma once

#include "Texture.h"

#include <ctime>

namespace Game
{
	class Button
	{
	private:
		Texture* mTexture;

		SDL_Rect button_rectangle,
			original_position,
			clip;

		float scale = 1.0f;

		bool has_been_clicked = false;
		bool is_clicked = false;

		void AdjustButtonSize(bool mouse_on_button) noexcept;

	public:
		void Update(void) noexcept;
		void Render(void) noexcept;

		bool IsClicked(void) noexcept;
		bool IsHeldDown(void) noexcept;

		void SetClip(SDL_Rect new_clip);

		Button(SDL_Rect clip, SDL_Rect button_rect, float scale) noexcept;
	};
}