#include "main.hpp"

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;

	tsee::log::log("Creating TSEE");
	tsee::TSEE game;
	for (size_t i = 0; i < 1; i++) {
		tsee::log::log("Adding Window");
		game.CreateWindow(rand() % 800, rand() % 640, "Test Window");
		game.CreateWindow(rand() % 800, rand() % 640);
		game.CreateWindow();
	}

	for (size_t i = 0; i < 25; i++) {
		tsee::log::log("Rendering");
		game.RenderAll();
		SDL_Delay(100);
	}

	return 0;
}