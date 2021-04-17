#define _CRT_SECURE_NO_WARNINGS

#include "NumberDisplay.h"
#include "Resources.h"
#include "Renderer.h"

SDL_Rect digit_clips[12];

#define MINUS_SIGN 10
#define EMPTY_DIGIT 11

Game::NumberDisplay::NumberDisplay(int starting_val, SDL_Rect display_rect)
{
	z_index = 4;

	this->value = starting_val;
	this->display_rect = display_rect;

	mTexture = Resources::Get()->gTextures.Get("digits");

	for (uint16_t i = 0; i < 12; ++i)
		digit_clips[i] = { 13 * i + i, 0, 13, 23 };
}

void Game::NumberDisplay::Render(void)
{
	char buffer[4];
	_itoa(value, buffer, 10);
	size_t len = strlen(buffer);
	
	RenderEmptyDigits(len);
	RenderNumber(buffer, len);
}

void Game::NumberDisplay::RenderEmptyDigits(size_t len)
{
	const int digit_width = display_rect.w / 3;

	for (size_t i = 0; i < 3 - len; ++i)
	{
		int x_pos = display_rect.x + i * (display_rect.w / 3);

		mTexture->Render({ x_pos , display_rect.y, digit_width, display_rect.h }, &digit_clips[EMPTY_DIGIT]);
	}
}

void Game::NumberDisplay::RenderNumber(char* buffer, size_t len)
{
	const int digit_width = display_rect.w / 3;

	for (signed int i = 0; i < len; ++i)
	{
		int clip_index;

		if (buffer[i] == '-')
		{
			clip_index = MINUS_SIGN;
		}

		else if (isdigit(buffer[i]))
		{
			clip_index = buffer[i] - '0';
		}

		int x_pos = display_rect.x + (i+3-len) * (display_rect.w / 3);

		mTexture->Render({ x_pos, display_rect.y, digit_width, display_rect.h }, &digit_clips[clip_index]);
	}
}

void Game::NumberDisplay::AddToValue(int additional_val)
{
	value += additional_val;
	RestrictValue();
}

void Game::NumberDisplay::SetValue(int new_val)
{
	value = new_val;
	RestrictValue();
}

/* Must be 3 characters long */
void Game::NumberDisplay::RestrictValue(void)
{
	if (value > 999)
	{
		value = 999;
	}

	if (value < -99)
	{
		value = -99;
	}
}

void Game::NumberDisplay::Update(void)
{
	Renderer::Get()->SubmitToRender(this);
}