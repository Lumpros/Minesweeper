#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <cstdio>
#include <ctime>

#include "Texture.h"
#include "Preprocessors.h"
#include "Sound.h"
#include "Resources.h"
#include "Exception.h"
#include "Minefield.h"
#include "Button.h"

namespace Game
{
	class Application
	{
	private:
		SDL_Window* mWindow = nullptr;
		Minefield* minefield = nullptr;
		Button* reset_button = nullptr;

	private:
		void Update(void) const noexcept;
		void Render(void) const noexcept;

		void LoadResources(void) const noexcept;

		void CapFramerate(clock_t clock_before_frame);

		void InitializeGameplayRelated(void);

		void InitializeSDL(void);
		void InitializeIMG(void);
		void InitializeAudio(void);
		void InitializeFonts(void);
		void CreateWindow(void);
		void CreateRenderer(void);

	public:
		void Initialize(void);

		void Loop(void) noexcept;

		~Application(void) noexcept;
	};
}

