#include "include/tsee.h"

TSEE TSEECreate(int width, int height) {
	TSEE tsee;
	tsee.fonts = NULL;
	tsee.window = malloc(sizeof(*tsee.window));
	tsee.window->width = width;
	tsee.window->height = height;
	tsee.window->running = false;
	tsee.window->fps = 60;
	return tsee;
}

bool TSEEClose(TSEE *tsee) {
	tsee->window->running = false;
	TSEEDestroyWindow(tsee->window);
	return true;
}