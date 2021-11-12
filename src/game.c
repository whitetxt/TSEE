#include "include/main.h"

SDL_Window *win;
SDL_Renderer *renderer;
int width;
int height;

// Initialises SDL, SDL_TTF and creates a window and renderer.
// @return 0 on success, 1 on failure.
int initWindow() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { 
		printf("Error initializing SDL: %s\n", SDL_GetError());
		return 1;
	}

	if(TTF_Init() == -1) {
		printf("Error initialising SDL_ttf: %s\n", TTF_GetError());
		return 1;
	}

	// Sets the flags for the window: Allows the window to be resized and allows it to be used on a high DPI screen.
	Uint32 winFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;

	win = SDL_CreateWindow("EvEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Settings.width, Settings.height, winFlags);
	width = Settings.width;
	height = Settings.height;

	// Checks if the window was created successfully.
	if (win == NULL) {
		printf("SDL had an error creating the window: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	SDL_SetWindowIcon(win, IMG_Load("textures/EvEngine.png"));
	
	printf("Initialised Window.\n");

	// Sets the flags for the renderer: Turns on vsync and uses hardware acceleration.
	Uint32 rendFlags = SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED;
	
	renderer = SDL_CreateRenderer(win, -1, rendFlags);

	// Checks if the renderer was created successfully.
	if (renderer == NULL) {
		printf("SDL had an error creating the renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	return 0;
}