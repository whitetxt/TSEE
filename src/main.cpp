#include "main.hpp"

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;

	tsee::log::log("Creating TSEE");
	tsee::TSEE game;
	tsee::log::log("Adding Window");
	game.CreateWindow(800, 640, "Test Window");
	tsee::log::log("Rendering");
	game.RenderAll();

	SDL_Delay(2500);
	tsee::log::log("Rendering");
	game.RenderAll();

	return 0;
}