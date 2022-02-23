#include "include/tsee.h"

TSEE *TSEECreate(int width, int height) {
	TSEELog("Initialising TSEE Engine...\n");
	TSEE *tsee = malloc(sizeof(*tsee));
	tsee->fonts = NULL;

	// Setup window + renderer
	tsee->window = malloc(sizeof(*tsee->window));
	tsee->window->width = width;
	tsee->window->height = height;
	tsee->window->running = true;
	tsee->window->fps = 60;

	// Setup world + textures
	tsee->world = malloc(sizeof(*tsee->world));
	tsee->world->objects = TSEEArrayCreate();
	tsee->world->physics_objects = TSEEArrayCreate();
	tsee->world->text = TSEEArrayCreate();
	tsee->world->parallax = TSEEArrayCreate();
	tsee->world->scroll_x = 0;
	tsee->world->scroll_y = 0;
	tsee->textures = TSEEArrayCreate();

	// Setup player
	tsee->player = malloc(sizeof(*tsee->player));
	tsee->player->physics_object = NULL;
	tsee->player->movement.up = false;
	tsee->player->movement.down = false;
	tsee->player->movement.left = false;
	tsee->player->movement.right = false;

	// Setup DT calculations
	tsee->dt = 0;
	tsee->last_time = 0;
	tsee->current_time = SDL_GetPerformanceCounter();
	
	// Setup UI
	tsee->ui = malloc(sizeof(*tsee->ui));

	// Setup init stuff
	tsee->init = malloc(sizeof(*tsee->init));
	tsee->init->text = false;
	tsee->init->ui = false;
	tsee->init->rendering = false;
	tsee->init->events = false;
	tsee->init->input = false;

	// Setup Debugging Counters
	tsee->debug = malloc(sizeof(*tsee->debug));
	tsee->debug->event_time = 0;
	tsee->debug->physics_time = 0;
	tsee->debug->render_time = 0;
	tsee->debug->frame_time = 0;
	tsee->debug->framerate = 0;
	tsee->debug->active = false;

	// Load basic settings
	TSEELoadSettings(tsee);
	TSEELog("TSEE Engine initialized.\n");
	return tsee;
}

bool TSEEInitAll(TSEE *tsee) {
	TSEELog("Initialising TSEE modules...\n");
	if (!TSEEInitRendering(tsee)) {
		TSEECritical("Failed to initialize TSEE Rendering Module.\n");
		TSEEClose(tsee);
		return false;
	}
	TSEELog("Initialized TSEE Rendering.\n");
	if (!TSEEInitText(tsee, true)) {
		TSEECritical("Failed to initialize TSEE Text Module.\n");
		TSEEClose(tsee);
		return false;
	}
	TSEELog("Initialized TSEE Text.\n");
	if (!TSEEInitEvents(tsee)) {
		TSEECritical("Failed to initialize TSEE Events Module.\n");
		TSEEClose(tsee);
		return false;
	}
	TSEELog("Initialized TSEE Events.\n");
	if (!TSEEInitInput(tsee)) {
		TSEECritical("Failed to initialize TSEE Input Module.\n");
		TSEEClose(tsee);
		return false;
	}
	TSEELog("Initialized TSEE Input.\n");
	if (!TSEEInitUI(tsee)) {
		TSEECritical("Failed to initialize TSEE UI Module.\n");
		TSEEClose(tsee);
		return false;
	}
	TSEELog("Initialized TSEE UI.\n");
	TSEELog("All TSEE modules initialized.\n");
	return true;
}

bool TSEEClose(TSEE *tsee) {
	tsee->window->running = false;
	for (size_t i = 0; i < tsee->world->physics_objects->size; i++) {
		TSEE_Physics_Object *pobj = TSEEArrayGet(tsee->world->physics_objects, i);
		TSEEDestroyPhysicsObject(pobj, false, false);
	}
	TSEEDestroyArray(tsee->world->physics_objects);
	for (size_t i = 0; i < tsee->world->objects->size; i++) {
		TSEE_Object *obj = TSEEArrayGet(tsee->world->objects, i);
		TSEEDestroyObject(obj, false);
	}
	TSEEDestroyArray(tsee->world->objects);
	for (size_t i = 0; i < tsee->world->text->size; i++) {
		TSEE_Text *text = TSEEArrayGet(tsee->world->text, i);
		TSEEDestroyText(text, true);
	}
	TSEEDestroyArray(tsee->world->text);
	for (size_t i = 0; i < tsee->world->parallax->size; i++) {
		TSEE_Parallax *para = TSEEArrayGet(tsee->world->parallax, i);
		TSEEDestroyParallax(para, false);
	}
	TSEEDestroyArray(tsee->world->parallax);
	for (size_t i = 0; i < tsee->textures->size; i++) {
		TSEE_Texture *tex = TSEEArrayGet(tsee->textures, i);
		TSEEDestroyTexture(tex);
	}
	TSEEDestroyArray(tsee->textures);
	TSEEUnloadAllFonts(tsee);
	TSEEDestroyWindow(tsee->window);
	free(tsee->player);
	free(tsee->world);
	free(tsee->window->title);
	free(tsee->window);
	free(tsee->events->event);
	free(tsee->events);
	free(tsee->ui);
	if (tsee->init->text)
		TTF_Quit();
	if (tsee->init->rendering) {
		IMG_Quit();
		SDL_Quit();
	}
	free(tsee->init);
	free(tsee->debug);
	free(tsee);
	return true;
}

bool TSEECalculateDT(TSEE *tsee) {
	tsee->last_time = tsee->current_time;
	tsee->current_time = SDL_GetPerformanceCounter();
	tsee->dt = (float) ( (tsee->current_time - tsee->last_time) / (float) SDL_GetPerformanceFrequency() );
	return true;
}

bool TSEESetWorldGravity(TSEE *tsee, float gravity) {
	tsee->world->gravity = gravity;
	return true;
}

bool TSEEReadyToRender(TSEE *tsee) {
	float timeBetweenFrames = 1.0f / tsee->window->fps;
	float dt = (float) ( (SDL_GetPerformanceCounter() - tsee->window->lastRender) / (float) SDL_GetPerformanceFrequency() );
	return dt >= timeBetweenFrames;
}