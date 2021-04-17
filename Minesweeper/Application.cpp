#include "Application.h"
#include "Renderer.h"

SDL_Renderer* Game::gRenderer = nullptr;

void Game::Application::Loop(void) noexcept
{	
	clock_t clock_before_frame;
	bool should_quit = false;
	SDL_Event sdl_event;

	while (!should_quit)
	{
		clock_before_frame = clock();
		while (SDL_PollEvent(&sdl_event))
			if (sdl_event.type == SDL_QUIT)
				should_quit = true;

		Update();
		Render();

		CapFramerate(clock_before_frame);
	}
} 

void Game::Application::Update(void) const noexcept
{

	minefield->Update();
}

void Game::Application::Render(void) const noexcept
{
	Renderer::Get()->RenderSubmittedItems();
}

void Game::Application::CapFramerate(clock_t clock_before_frame)
{
	clock_t ticksElapsed = clock() - clock_before_frame;
	if (ticksElapsed < SCREEN_TICKS_PER_FRAME)
		SDL_Delay(SCREEN_TICKS_PER_FRAME - ticksElapsed);
}

void Game::Application::LoadResources(void) const noexcept
{
	Resources* resources = Resources::Get();

	resources->gTextures.Add("cells", new Texture("Media/cells.png"));
	resources->gTextures.Add("button", new Texture("Media/button.png"));
	resources->gTextures.Add("field", new Texture("Media/field.png"));
}

Game::Application::~Application(void) noexcept
{
	delete reset_button;
	delete minefield;

	Resources::Get()->ReleaseResources();

	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(gRenderer);

	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	Mix_CloseAudio();
}

void Game::Application::Initialize(void)
{
	InitializeSDL();
	InitializeIMG();
	InitializeAudio();
	InitializeFonts();
	
	CreateWindow();
	CreateRenderer();

	LoadResources();

	InitializeGameplayRelated();

	SDL_ShowWindow(mWindow);
}

void Game::Application::InitializeSDL(void)
{
	if ((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0))
		ThrowErrorUsingSDLMessage("Failed to Initialize SDL! ", SDL_GetError());

	/* Disable pixel art blur */
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"))
		puts("Warning: Linear texture filtering not enabled!");
}

void Game::Application::InitializeIMG(void)
{
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
		ThrowErrorUsingSDLMessage("SDL_image could not initialize! IMG_Erro: ", IMG_GetError());
}

void Game::Application::InitializeAudio(void)
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		ThrowErrorUsingSDLMessage("SDL_mixer could not initialize! Mix_Error: ", Mix_GetError());
}

void Game::Application::InitializeFonts(void)
{
	constexpr int FAILED = -1;
	if (TTF_Init() == FAILED)
		ThrowErrorUsingSDLMessage("SDL_ttf could not initialize! TTF_Error:", TTF_GetError());
}

void Game::Application::CreateWindow(void)
{
	mWindow = SDL_CreateWindow(
		"Minesweeper", 
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, 
		WINDOW_WIDTH, 
		WINDOW_HEIGHT, 
		SDL_WINDOW_HIDDEN
	);

	if (!mWindow)
		ThrowErrorUsingSDLMessage("Failed to create SDL window! SDL_Error: ", SDL_GetError());
}

void Game::Application::CreateRenderer(void)
{
	gRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!gRenderer)
		ThrowErrorUsingSDLMessage("Failed to create renderer! SDL_Error: ", SDL_GetError());
}

void Game::Application::InitializeGameplayRelated(void)
{
	SDL_Rect button_position = {
		WINDOW_WIDTH / 2 - 48 / 2,
		30, 48, 48,
	};

	reset_button = new Button({ 0, 1, 24, 24 }, button_position, 1.0f);

	minefield = new Minefield();
	minefield->SetPositionInWindow(SDL_Point{ 24, 110 });
	minefield->RegisterResetButton(reset_button);
}