#include "Application.h"

int main(int argc, char* argv[])
{
	Game::Application app;
	try {
		app.Initialize();
		app.Loop();
	}
	catch (std::exception& e) {
		puts(e.what());
	}

	return 0;
}