#include "include/tsee.h"

TSEE createTSEEGame(int width, int height) {
	TSEE tsee;
	tsee.window = malloc(sizeof(*tsee.window));
	tsee.window->width = width;
	tsee.window->height = height;
	tsee.window->running = false;
	tsee.window->fps = 60;
	return tsee;
}