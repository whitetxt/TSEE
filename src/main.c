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

void create_menu(TSEE *tsee) {
	TSEE_Toolbar_AddButton(tsee, "_default", "Home");
	TSEE_Toolbar_AddChild(tsee, "Home", "_default", "Quit", quitGame);
	TSEE_Toolbar_AddButton(tsee, "_default", "Maps");
	TSEE_Toolbar_AddChild(tsee, "Maps", "_default", "Save", saveMap);
	TSEE_Toolbar_AddChild(tsee, "Maps", "_default", "Load", loadMap);
}

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;

	TSEE *tsee = TSEE_Create(800, 600);

	if (!TSEE_InitAll(tsee)) {
		TSEE_Critical("Failed to initialize TSEE\n");
		return -1;
	}
	if (!TSEE_Window_SetTitle(tsee, "TSEE - Example Game")) {
		TSEE_Warn("Failed to set window title.\n");
	}

	create_menu(tsee);

	TSEE_Object_Create(tsee, TSEE_Texture_Create(tsee, "assets/test_image.png"),
					   TSEE_ATTRIB_PHYS | TSEE_ATTRIB_PLAYER, 200, 300);
	TSEE_Player_SetJumpForce(tsee, 250);
	TSEE_Player_SetSpeed(tsee, 500);
	TSEE_Player_SetStepSize(tsee, 5);
	TSEE_World_SetGravity(tsee, (TSEE_Vec2){0, -50});
	TSEE_Object_Create(tsee, TSEE_Texture_Create(tsee, "assets/test_image.png"),
					   TSEE_ATTRIB_STATIC, 160, 30);
	TSEE_Object_Create(tsee, TSEE_Texture_Create(tsee, "assets/test_image.png"),
					   TSEE_ATTRIB_STATIC, 170, 35);
	TSEE_Object_Create(tsee, TSEE_Texture_Create(tsee, "assets/test_image.png"),
					   TSEE_ATTRIB_STATIC, 180, 40);
	TSEE_Object_Create(tsee, TSEE_Texture_Create(tsee, "assets/test_image.png"),
					   TSEE_ATTRIB_STATIC, 190, 45);
	TSEE_Parallax_Create(tsee,
						 TSEE_Texture_Create(tsee, "assets/parallax1.png"), 6);
	TSEE_Parallax_Create(tsee,
						 TSEE_Texture_Create(tsee, "assets/parallax2.png"), 4);
	TSEE_Parallax_Create(tsee,
						 TSEE_Texture_Create(tsee, "assets/parallax3.png"), 2);
	TSEE_Parallax_Create(tsee,
						 TSEE_Texture_Create(tsee, "assets/parallax4.png"), 1);

	for (int x = 0; x < tsee->window->width; x += 88) {
		TSEE_Object_Create(tsee,
						   TSEE_Texture_Create(tsee, "assets/test_image.png"),
						   TSEE_ATTRIB_STATIC, x, 25);
	}

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
		TSEE_Error(
			"Failed to save settings. All changes will be lost.\nSorry.");
	}
	TSEE_Log("Closing.\n");
	TSEE_Close(tsee);
	return 0;
}