typedef struct TSEE_Window {
	SDL_Window *window;
	SDL_Renderer *renderer;
	int width;
	int height;
	int fps;
	bool running;
	char *title;
} TSEE_Window;

typedef struct TSEE {
	TSEE_Window *window;
} TSEE;