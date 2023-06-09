// TSEE's window type wrapper for SDL2, created along with the TSEE object in
// TSEE_Create(width, height).
typedef struct TSEE_Window {
	SDL_Window *window;
	SDL_Renderer *renderer;
	int width;
	int height;
	int fps;
	bool vsync;
	bool running;
	char *title;
	SDL_Point mouse;
	SDL_DisplayMode mode;
	Uint64 last_render;
} TSEE_Window;
