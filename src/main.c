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

	TSEE_Array *test = TSEE_Array_Create();
	int a = 1, b = 2, c = 3;
	TSEE_Array_Append(test, &a);
	TSEE_Array_Append(test, &c);
	TSEE_Array_Insert(test, &b, 1);


	TSEE *tsee = TSEE_Create(800, 600);

	if (!TSEE_InitAll(tsee)) {
		TSEE_Critical("Failed to initialize TSEE\n");
		TSEE_Close(tsee);
		return -1;
	}
	if (!TSEE_Window_SetTitle(tsee, "TSEE - Example Game")) {
		TSEE_Warn("Failed to set window title.\n");
	}

	TSEE_Object_Create(tsee, TSEE_Texture_Create(tsee, "assets/test_image.png"), TSEE_ATTRIB_PHYS_ENABLED | TSEE_ATTRIB_PLAYER, 0, 0);
	TSEE_Player_SetJumpForce(tsee, 1.5);
	TSEE_Player_SetSpeed(tsee, 300);
	TSEE_World_SetGravity(tsee, (TSEE_Vec2){0, -9.81});
	TSEE_Object_Create(tsee, TSEE_Texture_Create(tsee, "assets/test_image.png"), TSEE_ATTRIB_NONE, 160, 595);
	TSEE_Object_Create(tsee, TSEE_Texture_Create(tsee, "assets/test_image.png"), TSEE_ATTRIB_NONE, 170, 590);
	TSEE_Object_Create(tsee, TSEE_Texture_Create(tsee, "assets/test_image.png"), TSEE_ATTRIB_NONE, 180, 585);
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
			TSEE_Physics_PerformStep(tsee);
		}*/
		/*
		For single physics step per frame*/
		TSEE_Events_Handle(tsee);
		TSEE_CalculateDT(tsee);
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