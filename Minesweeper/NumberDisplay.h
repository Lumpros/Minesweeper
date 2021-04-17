#pragma once

#include "Texture.h"
#include "Entity.h"

namespace Game
{
	// 3 character wide display
	class NumberDisplay : public Renderable
	{
	private:
		Texture* mTexture = nullptr;

		int value = 0;
		SDL_Rect display_rect;

		void RestrictValue(void);
		void RenderEmptyDigits(size_t len);
		void RenderNumber(char* buf, size_t len);

	public:
		NumberDisplay(int starting_val, SDL_Rect display_rect);

		void AddToValue(int additional_val);
		void SetValue(int new_val);

		void Render(void) override;
		void Update(void);
	};
}

