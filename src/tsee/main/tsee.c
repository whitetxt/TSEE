#include "../tsee.h"

/**
 * @brief Creates a TSEE object
 * 
 * @param width Window width
 * @param height Window height
 * @return TSEE* 
 */
TSEE *TSEE_Create(int width, int height) {
	TSEE_Log("Initialising TSEE Engine...\n");
	TSEE *tsee = xmalloc(sizeof(*tsee));
	tsee->fonts = NULL;

	// Setup window + renderer
	tsee->window = xmalloc(sizeof(*tsee->window));
	tsee->window->width = width;
	tsee->window->height = height;
	tsee->window->running = true;
	tsee->window->fps = 60;
	tsee->window->last_render = 0;

	// Setup world + textures
	tsee->world = xmalloc(sizeof(*tsee->world));
	tsee->world->objects = TSEE_Array_Create();
	tsee->world->scroll_x = 0;
	tsee->world->scroll_y = 0;
	tsee->textures = TSEE_Array_Create();

	// Setup player
	tsee->player = xmalloc(sizeof(*tsee->player));
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
	tsee->ui = xmalloc(sizeof(*tsee->ui));

	// Setup init stuff
	tsee->init = xmalloc(sizeof(*tsee->init));
	tsee->init->text = false;
	tsee->init->ui = false;
	tsee->init->rendering = false;
	tsee->init->events = false;
	tsee->init->input = false;

	// Setup Debugging Counters
	tsee->debug = xmalloc(sizeof(*tsee->debug));
	tsee->debug->event_time = 0;
	tsee->debug->physics_time = 0;
	tsee->debug->render_time = 0;
	tsee->debug->frame_time = 0;
	tsee->debug->framerate = 0;
	tsee->debug->active = false;

	// Load basic settings
	TSEE_Settings_Load(tsee);
	TSEE_Log("TSEE Engine initialized.\n");
	return tsee;
}

/**
 * @brief Initialises all subsystems for a TSEE.
 * 
 * @param tsee TSEE to initialise for.
 * @return true on success, false on fail.
 */
bool TSEE_InitAll(TSEE *tsee) {
	TSEE_Log("Initialising TSEE modules...\n");
	if (!TSEE_Rendering_Init(tsee)) {
		TSEE_Critical("Failed to initialize TSEE Rendering Module.\n");
		TSEE_Close(tsee);
		return false;
	}
	TSEE_Log("Initialized TSEE Rendering.\n");
	if (!TSEE_Text_Init(tsee, true)) {
		TSEE_Critical("Failed to initialize TSEE Text Module.\n");
		TSEE_Close(tsee);
		return false;
	}
	TSEE_Log("Initialized TSEE Text.\n");
	if (!TSEE_Events_Init(tsee)) {
		TSEE_Critical("Failed to initialize TSEE Events Module.\n");
		TSEE_Close(tsee);
		return false;
	}
	TSEE_Log("Initialized TSEE Events.\n");
	if (!TSEE_Input_Init(tsee)) {
		TSEE_Critical("Failed to initialize TSEE Input Module.\n");
		TSEE_Close(tsee);
		return false;
	}
	TSEE_Log("Initialized TSEE Input.\n");
	if (!TSEE_UI_Init(tsee)) {
		TSEE_Critical("Failed to initialize TSEE UI Module.\n");
		TSEE_Close(tsee);
		return false;
	}
	TSEE_Log("Initialized TSEE UI.\n");
	TSEE_Log("All TSEE modules initialized.\n");
	return true;
}

/**
 * @brief Closes a TSEE object, freeing all memory used.
 * 
 * @param tsee TSEE object to close.
 * @return true on success, false on fail.
 */
bool TSEE_Close(TSEE *tsee) {
	tsee->window->running = false;
	for (size_t i = 0; i < tsee->world->objects->size; i++) {
		TSEE_Object *obj = TSEE_Array_Get(tsee->world->objects, i);
		TSEEDestroyObject(obj, false);
	}
	TSEE_Array_Destroy(tsee->world->objects);
	for (size_t i = 0; i < tsee->textures->size; i++) {
		TSEE_Texture *tex = TSEE_Array_Get(tsee->textures, i);
		TSEE_Texture_Destroy(tex);
	}
	TSEE_Array_Destroy(tsee->textures);
	TSEE_Font_UnloadAll(tsee);
	
	xfree(tsee->player);
	xfree(tsee->world);
	
	xfree(tsee->events->event);
	xfree(tsee->events);
	for (size_t i = 0; i < tsee->ui->toolbar->size; i++) {
		TSEE_Toolbar_Object *obj = TSEE_Array_Get(tsee->ui->toolbar, i);
		TSEE_Text_Destroy(obj->text, true);
		for (size_t j = 0; j < obj->buttons->size; j++) {
			TSEE_Toolbar_Child *child = TSEE_Array_Get(obj->buttons, j);
			TSEE_Text_Destroy(child->text, true);
			xfree(child);
		}
		TSEE_Array_Destroy(obj->buttons);
		xfree(obj);
	}
	TSEE_Array_Destroy(tsee->ui->toolbar);
	xfree(tsee->ui);

	TSEE_Window_Destroy(tsee->window);
	xfree(tsee->window);
	if (tsee->init->text)
		TTF_Quit();
	if (tsee->init->rendering) {
		IMG_Quit();
		SDL_Quit();
	}
	xfree(tsee->init);
	xfree(tsee->debug);
	xfree(tsee);
	return true;
}

/**
 * @brief Calculates delta-time for a TSEE object.
 * 
 * @param tsee TSEE object to calculate delta-time for.
 * @return true on success, false on fail.
 */
bool TSEE_CalculateDT(TSEE *tsee) {
	tsee->last_time = tsee->current_time;
	tsee->current_time = SDL_GetPerformanceCounter();
	tsee->dt = (float) ( (tsee->current_time - tsee->last_time) / (float) SDL_GetPerformanceFrequency() );
	return true;
}

/**
 * @brief Sets the gravity strength for a TSEE object
 * 
 * @param tsee TSEE object to set gravity for.
 * @param gravity New gravity value.
 * @return true on success, false on fail.
 */
bool TSEE_World_SetGravity(TSEE *tsee, float gravity) {
	tsee->world->gravity = gravity;
	return true;
}