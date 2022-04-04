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

	int idx = TSEECreatePhysicsObject(tsee, TSEE_Texture_Create(tsee, "assets/test_image.png"), 1);
	TSEE_Player_Create(tsee, TSEE_Array_Get(tsee->world->physics_objects, idx));
	TSEE_Player_SetJumpForce(tsee, 60);
	TSEE_Player_SetSpeed(tsee, 1500);
	TSEE_World_SetGravity(tsee, 800);
	idx = TSEE_Create_Object(tsee, TSEE_Texture_Create(tsee, "assets/test_image.png"));
	TSEESetObjectPosition(tsee, idx, 160, 595);
	idx = TSEE_Create_Object(tsee, TSEE_Texture_Create(tsee, "assets/test_image.png"));
	TSEESetObjectPosition(tsee, idx, 170, 590);
	idx = TSEE_Create_Object(tsee, TSEE_Texture_Create(tsee, "assets/test_image.png"));
	TSEESetObjectPosition(tsee, idx, 180, 585);
	TSEE_Parallax_Create(tsee, TSEE_Texture_Create(tsee, "assets/parallax1.png"), 6);
	TSEE_Parallax_Create(tsee, TSEE_Texture_Create(tsee, "assets/parallax2.png"), 4);
	TSEE_Parallax_Create(tsee, TSEE_Texture_Create(tsee, "assets/parallax3.png"), 2);
	TSEE_Parallax_Create(tsee, TSEE_Texture_Create(tsee, "assets/parallax4.png"), 1);
	TSEE_Toolbar_AddButton(tsee, "_default", "Home");
	TSEE_Toolbar_AddChild(tsee, "Home", "_default", "Quit", quitGame);
	TSEE_Toolbar_AddButton(tsee, "_default", "Maps");
	TSEE_Toolbar_AddChild(tsee, "Maps", "_default", "Save", saveMap);
	TSEE_Toolbar_AddChild(tsee, "Maps", "_default", "Load", loadMap);

	while (tsee->window->running) {
		/*
		For multiple physics steps per frame
		while (!TSEE_Rendering_IsReady(tsee)) {
			TSEE_Events_Handle(tsee);
			TSEE_CalculateDT(tsee);
			TSEEPerformPhysics(tsee);
		}*/
		/*
		For single physics step per frame*/
		TSEE_Events_Handle(tsee);
		TSEE_CalculateDT(tsee);
		TSEEPerformPhysics(tsee);
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