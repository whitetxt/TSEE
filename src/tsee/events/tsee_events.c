#include "../tsee.h"

bool TSEE_Events_Init(TSEE *tsee) {
	if (tsee->init->events) {
		return true;
	}
	// Setup events
	tsee->events = xmalloc(sizeof(*tsee->events));
	tsee->events->event = xmalloc(sizeof(*tsee->events->event));
	tsee->events->keypress = NULL;
	tsee->events->keyrelease = NULL;
	tsee->events->mousemotion = NULL;
	tsee->events->mouseclick = NULL;
	tsee->init->events = true;
	return true;
}

bool TSEE_Events_Handle(TSEE *tsee) {
	Uint64 start = SDL_GetPerformanceCounter();
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
			case SDL_MOUSEMOTION:
				tsee->window->mouse = (SDL_Point){tsee->events->event->motion.x, tsee->events->event->motion.y};
				if (tsee->events->mousemotion) {
					tsee->events->mousemotion(tsee, tsee->events->event->motion.x, tsee->events->event->motion.y);
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (tsee->events->event->button.button == SDL_BUTTON_LEFT) {TSEE_UI_Click(tsee, tsee->events->event->button.x, tsee->events->event->button.y);} // Checks if the UI is clicked
				if (tsee->events->mouseclick) {
					tsee->events->mouseclick(tsee, tsee->events->event->button.x, tsee->events->event->button.y, tsee->events->event->button.button);
				}
				break;
		}
	}
	Uint64 end = SDL_GetPerformanceCounter();
	tsee->debug->event_time += (end - start) * 1000/ (double) SDL_GetPerformanceFrequency();
	return true;
}