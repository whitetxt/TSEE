#include "include/tsee.h"

bool TSEEInitRendering(TSEE *tsee) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	tsee->window->window = SDL_CreateWindow("No window title set: Call TSEESetWindowTitle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, tsee->window->width, tsee->window->height, SDL_WINDOW_SHOWN);
	if (tsee->window->window == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		SDL_Exit();
		return false;
	}

	tsee->window->renderer = SDL_CreateRenderer(tsee->window->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (tsee->window->renderer == NULL) {
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(tsee->window->window);
		SDL_Exit();
		return false;
	}

	return true;
};

void TSEEDestroyWindow(TSEE_Window *window) {
	SDL_DestroyRenderer(window->renderer);
	SDL_DestroyWindow(window->window);
	return true;
}

bool TSEESetWindowTitle(TSEE *tsee, char *title) {
	if (tsee->window->window == NULL) {
		TSEEError("Window is not initialized!\n");
		return false;
	}

	SDL_SetWindowTitle(tsee->window->window, title);
	return true;
};