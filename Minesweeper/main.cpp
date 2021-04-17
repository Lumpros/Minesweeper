#include "Application.h"

#include <Windows.h>

int main(int argc, char* argv[])
{
	Game::Application app;
	try {
		app.Initialize();
		app.Loop();
	}
	catch (std::exception& e) {
		MessageBoxA(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
	} 

	return 0;
}