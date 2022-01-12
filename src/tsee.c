#include "include/tsee.h"

TSEE TSEECreate(int width, int height) {
	TSEELog("Initialising TSEE Engine...\n");
	TSEE tsee;
	tsee.fonts = NULL;

	// Setup window + renderer
	tsee.window = malloc(sizeof(*tsee.window));
	tsee.window->width = width;
	tsee.window->height = height;
	tsee.window->running = true;
	tsee.window->fps = 60;

	// Setup world + textures
	tsee.world = malloc(sizeof(*tsee.world));
	tsee.world->objects = TSEEArrayCreate();
	tsee.world->physics_objects = TSEEArrayCreate();
	tsee.world->text = TSEEArrayCreate();
	tsee.world->parallax = TSEEArrayCreate();
	tsee.world->scroll_x = 0;
	tsee.world->scroll_y = 0;
	tsee.textures = TSEEArrayCreate();

	// Setup player
	tsee.player = malloc(sizeof(*tsee.player));
	tsee.player->physics_object = NULL;
	tsee.player->movement.up = false;
	tsee.player->movement.down = false;
	tsee.player->movement.left = false;
	tsee.player->movement.right = false;

	// Setup DT calculations
	tsee.dt = 0;
	tsee.last_time = 0;
	tsee.current_time = SDL_GetPerformanceCounter();
	
	// Setup UI
	tsee.ui = malloc(sizeof(*tsee.ui));

	// Load basic settings
	TSEELoadSettings(&tsee);
	TSEELog("TSEE Engine initialised.\n");
	return tsee;
}

bool TSEEInitAll(TSEE *tsee) {
	TSEELog("Initialising TSEE modules...\n");
	if (!TSEEInitRendering(tsee)) {
		TSEECritical("Failed to initialize TSEE Rendering Module.\n");
		TSEEClose(tsee);
		return false;
	}
	TSEELog("Initialised TSEE Rendering.\n");
	if (!TSEEInitText(tsee, true)) {
		TSEECritical("Failed to initialize TSEE Text Module.\n");
		TSEEClose(tsee);
		return false;
	}
	TSEELog("Initialised TSEE Text.\n");
	if (!TSEEInitEvents(tsee)) {
		TSEECritical("Failed to initialize TSEE Events Module.\n");
		TSEEClose(tsee);
		return false;
	}
	TSEELog("Initialised TSEE Events.\n");
	if (!TSEEInitInput(tsee)) {
		TSEECritical("Failed to initialize TSEE Input Module.\n");
		TSEEClose(tsee);
		return false;
	}
	TSEELog("Initialised TSEE Input.\n");
	if (!TSEEInitUI(tsee)) {
		TSEECritical("Failed to initialize TSEE UI Module.\n");
		TSEEClose(tsee);
		return false;
	}
	TSEELog("Initialised TSEE UI.\n");
	TSEELog("All TSEE modules initialised.\n");
	return true;
}

bool TSEEClose(TSEE *tsee) {
	tsee->window->running = false;
	TSEEArrayFree(tsee->world->objects);
	TSEEArrayFree(tsee->world->physics_objects);
	TSEEArrayFree(tsee->world->text);
	TSEEArrayFree(tsee->world->parallax);
	TSEEArrayFree(tsee->textures);
	TSEEDestroyWindow(tsee->window);
	return true;
}

bool TSEECalculateDT(TSEE *tsee) {
	tsee->last_time = tsee->current_time;
	tsee->current_time = SDL_GetPerformanceCounter();
	tsee->dt = (double) ( (tsee->current_time - tsee->last_time) * 1000 / (double) SDL_GetPerformanceFrequency() );
	tsee->dt = tsee->dt * 0.001; // Convert from ms to s
	return true;
}

bool TSEESetWorldGravity(TSEE *tsee, double gravity) {
	tsee->world->gravity = gravity;
	return true;
}