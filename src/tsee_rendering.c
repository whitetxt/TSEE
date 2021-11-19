#include "include/tsee.h"

bool TSEEInitRendering(TSEE *tsee) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		TSEEError("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP) == 0) {
		TSEEError("SDL_IMG could not initialise! IMG_Error: %s\n", IMG_GetError());
		return false;
	}

	tsee->window->window = SDL_CreateWindow("No window title set: Call TSEESetWindowTitle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, tsee->window->width, tsee->window->height, SDL_WINDOW_SHOWN);
	if (tsee->window->window == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		SDL_Quit();
		return false;
	}

	tsee->window->renderer = SDL_CreateRenderer(tsee->window->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (tsee->window->renderer == NULL) {
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(tsee->window->window);
		SDL_Quit();
		return false;
	}

	return true;
};

void TSEEWindowUpdateSize(TSEE *tsee) {
	SDL_SetWindowSize(tsee->window->window, tsee->window->width, tsee->window->height);
}

void TSEEDestroyWindow(TSEE_Window *window) {
	SDL_DestroyRenderer(window->renderer);
	SDL_DestroyWindow(window->window);
}

bool TSEESetWindowTitle(TSEE *tsee, char *title) {
	if (tsee->window->window == NULL) {
		TSEEError("Window is not initialized!\n");
		return false;
	}

	SDL_SetWindowTitle(tsee->window->window, title);
	tsee->window->title = strdup(title);
	return true;
};

bool TSEERenderAll(TSEE *tsee) {
	SDL_RenderClear(tsee->window->renderer);
	// Render parallax backgrounds
	TSEERenderParallax(tsee);

	// Render all objects
	for (size_t i = 0; i < tsee->world->objects->size; i++) {
		TSEE_Object *obj = TSEEArrayGet(tsee->world->objects, i);
		TSEERenderObject(tsee, obj);
	}

	// Render all text
	for (size_t i = 0; i < tsee->world->text->size; i++) {
		TSEE_Text *text = TSEEArrayGet(tsee->world->text, i);
		TSEERenderText(tsee, text);
	}

	SDL_RenderPresent(tsee->window->renderer);
	return true;
}