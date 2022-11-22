// TSEE's event handling system.
typedef struct TSEE_Events {
	SDL_Event *event;
	void (*keypress)(void *tsee, SDL_Keycode keycode);
	void (*keyrelease)(void *tsee, SDL_Keycode keycode);
	void (*mousemotion)(void *tsee, int x, int y);
	void (*mouseclick)(void *tsee, int x, int y, int button);
} TSEE_Events;