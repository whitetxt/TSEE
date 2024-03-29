#include "main.hpp"

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;

	tsee::log::debug("Creating TSEE");
	tsee::TSEE game;
	tsee::log::debug("Adding Window");
	auto win = game.CreateWindow();

	auto spr = win.CreateSprite("funny.jpg");

	for (size_t i = 0; i < 25; i++) {
		tsee::log::debug("Rendering");
		game.RenderAll();
		SDL_Delay(100);
	}

	return 0;
}