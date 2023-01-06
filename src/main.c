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

	TSEE_Object_Create(tsee, TSEE_Texture_Create(tsee, "assets/test_image2.png"),
					   TSEE_ATTRIB_PHYS | TSEE_ATTRIB_PLAYER, 300, 400);
	TSEE_Player_SetJumpForce(tsee, 50);
	TSEE_Player_SetSpeed(tsee, 750);
	TSEE_Player_SetStepSize(tsee, 5);
	TSEE_World_SetGravity(tsee, (TSEE_Vec2){0, -500});
	TSEE_Parallax_Create(tsee,
						 TSEE_Texture_Create(tsee, "assets/parallax1.png"), 6);
	TSEE_Parallax_Create(tsee,
						 TSEE_Texture_Create(tsee, "assets/parallax2.png"), 4);
	//TSEE_Parallax_Create(tsee,
	//					 TSEE_Texture_Create(tsee, "assets/parallax3.png"), 2);
	//TSEE_Parallax_Create(tsee,
	//					 TSEE_Texture_Create(tsee, "assets/parallax4.png"), 1);

	printf("Breakpoint here\n");

	for (int x = 0; x < tsee->window->width; x += 64) {
		TSEE_Object_Create(tsee,
						   TSEE_Texture_Create(tsee, "assets/box.png"),
						   TSEE_ATTRIB_STATIC, x, 64);
	}
	//TSEE_Object_Create(tsee, TSEE_Texture_Create(tsee, "assets/test_image.png"), TSEE_ATTRIB_PHYS, -200, 500);
	//TSEE_Object_Create(tsee, TSEE_Texture_Create(tsee, "assets/box.png"), TSEE_ATTRIB_PHYS, 20, 500);
	//TSEE_Object_Create(tsee, TSEE_Texture_Create(tsee, "assets/box.png"), TSEE_ATTRIB_PHYS, 20, 600);

	// After setup for the map, correct dt
	tsee->current_time = SDL_GetPerformanceCounter();
	tsee->world->max_scroll_x = 1000;

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