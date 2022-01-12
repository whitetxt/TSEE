#include "include/tsee.h"

bool TSEEInitEvents(TSEE *tsee) {
	// Setup events
	tsee->events = malloc(sizeof(*tsee->events));
	tsee->events->event = malloc(sizeof(*tsee->events->event));
	tsee->events->keypress = NULL;
	tsee->events->keyrelease = NULL;
	tsee->init->events = true;
	return true;
}

bool TSEEHandleEvents(TSEE *tsee) {
	while (SDL_PollEvent(tsee->events->event)) {
		switch (tsee->events->event->type) {
			case SDL_QUIT:
				tsee->window->running = false;
				break;
			case SDL_KEYDOWN:
				if (tsee->events->keypress) {
					tsee->events->keypress(tsee, tsee->events->event->key.keysym.sym);
				}
				break;
			case SDL_KEYUP:
				if (tsee->events->keyrelease) {
					tsee->events->keyrelease(tsee, tsee->events->event->key.keysym.sym);
				}
				break;
		}
	}
	return true;
}