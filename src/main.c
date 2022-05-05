#define TSEE_DEV
#include "main.h"

void quitGame(void *t) {
	TSEE *tsee = (TSEE *)t;
	tsee->window->running = false;
}

void saveMap(void *t) {
	TSEE *tsee = (TSEE *)t;
	TSEE_Map_Save(tsee, "map.tsee_map");
}

void loadMap(void *t) {
	TSEE *tsee = (TSEE *)t;
	TSEE_Map_Load(tsee, "map.tsee_map");
}

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;

	TSEE *tsee = TSEE_Create(800, 600);

	if (!TSEE_InitAll(tsee)) {
		TSEE_Critical("Failed to initialize TSEE\n");
		TSEE_Close(tsee);
		return -1;
	}
	if (!TSEE_Window_SetTitle(tsee, "TSEE - Example Game")) {
		TSEE_Warn("Failed to set window title.\n");
	}

	TSEE_Object_Create(tsee, TSEE_Texture_Create(tsee, "assets/test_image.png"), TSEE_ATTRIB_PHYS_ENABLED | TSEE_ATTRIB_PLAYER, 200, 300);
	TSEE_Player_SetJumpForce(tsee, 0.1);
	TSEE_Player_SetSpeed(tsee, 9);
	TSEE_World_SetGravity(tsee, (TSEE_Vec2){0, -9.81});
	TSEE_Object_Create(tsee, TSEE_Texture_Create(tsee, "assets/test_image.png"), TSEE_ATTRIB_NONE, 160, 30);
	TSEE_Object_Create(tsee, TSEE_Texture_Create(tsee, "assets/test_image.png"), TSEE_ATTRIB_NONE, 170, 35);
	TSEE_Object_Create(tsee, TSEE_Texture_Create(tsee, "assets/test_image.png"), TSEE_ATTRIB_NONE, 180, 40);
	TSEE_Object_Create(tsee, TSEE_Texture_Create(tsee, "assets/test_image.png"), TSEE_ATTRIB_NONE, 190, 45);
	TSEE_Parallax_Create(tsee, TSEE_Texture_Create(tsee, "assets/parallax1.png"), 6);
	TSEE_Parallax_Create(tsee, TSEE_Texture_Create(tsee, "assets/parallax2.png"), 4);
	TSEE_Parallax_Create(tsee, TSEE_Texture_Create(tsee, "assets/parallax3.png"), 2);
	TSEE_Parallax_Create(tsee, TSEE_Texture_Create(tsee, "assets/parallax4.png"), 1);

	for (int x = 0; x < tsee->window->width; x += 32) {
		TSEE_Object_Create(tsee, TSEE_Texture_Create(tsee, "assets/test_image.png"), TSEE_ATTRIB_NONE, x, 25);
	}

	for (size_t i = 0; i < tsee->world->objects->size; i++) {
		TSEE_Object *obj = tsee->world->objects->data[i];
		if (!TSEE_Object_CheckAttribute(obj, TSEE_ATTRIB_PARALLAX)) continue;

		TSEE_Log("Parallax with distance: %f\n", obj->parallax.distance);
	}

	TSEE_Toolbar_AddButton(tsee, "_default", "Home");
	TSEE_Toolbar_AddChild(tsee, "Home", "_default", "Quit", quitGame);
	TSEE_Toolbar_AddButton(tsee, "_default", "Maps");
	TSEE_Toolbar_AddChild(tsee, "Maps", "_default", "Save", saveMap);
	TSEE_Toolbar_AddChild(tsee, "Maps", "_default", "Load", loadMap);

	// After setup for the map, correct dt
	tsee->current_time = SDL_GetPerformanceCounter();

	while (tsee->window->running) {		
		TSEE_Events_Handle(tsee);
		TSEE_CalculateDT(tsee);
		TSEE_Player_HandleInput(tsee);
		TSEE_Physics_PerformStep(tsee);
		TSEE_RenderAll(tsee);
	}

	TSEE_Log("Saving settings.\n");
	if (!TSEE_Settings_Save(tsee)) {
		TSEE_Error("Failed to save settings. All changes will be lost.\nSorry.");
	}
	TSEE_Log("Closing.\n");
	TSEE_Close(tsee);
	return 0; 
}