#include "include/main.h"

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;

	TSEE tsee = TSEECreate(800, 600);

	if (!TSEEInitAll(&tsee)) {
		TSEECritical("Failed to initialize TSEE\n");
		TSEEClose(&tsee);
		return -1;
	}

	if (!TSEESetWindowTitle(&tsee, "TSEE - Example Game")) {
		TSEEWarn("Failed to set window title.\n");
	}

	int idx = TSEECreatePhysicsObject(&tsee, TSEECreateTextureFromPath(&tsee, "assets/test_image.png"), 1);
	TSEECreatePlayer(&tsee, TSEEArrayGet(tsee.world->physics_objects, idx));
	TSEESetPlayerJumpForce(&tsee, 30);
	TSEESetPlayerSpeed(&tsee, 50);
	TSEESetWorldGravity(&tsee, 15);
	idx = TSEECreateObject(&tsee, TSEECreateTextureFromPath(&tsee, "assets/test_image.png"));
	TSEESetObjectPosition(&tsee, idx, 150, 600);
	idx = TSEECreateObject(&tsee, TSEECreateTextureFromPath(&tsee, "assets/test_image.png"));
	TSEESetObjectPosition(&tsee, idx, 160, 595);
	idx = TSEECreateObject(&tsee, TSEECreateTextureFromPath(&tsee, "assets/test_image.png"));
	TSEESetObjectPosition(&tsee, idx, 170, 590);
	idx = TSEECreateObject(&tsee, TSEECreateTextureFromPath(&tsee, "assets/test_image.png"));
	TSEESetObjectPosition(&tsee, idx, 180, 585);
	idx = TSEECreateObject(&tsee, TSEECreateTextureFromPath(&tsee, "assets/test_image.png"));
	TSEESetObjectPosition(&tsee, idx, 190, 580);
	idx = TSEECreateObject(&tsee, TSEECreateTextureFromPath(&tsee, "assets/test_image.png"));
	TSEESetObjectPosition(&tsee, idx, 200, 575);
	TSEECreateParallax(&tsee, TSEECreateTextureFromPath(&tsee, "assets/parallax1.png"), 6);
	TSEECreateParallax(&tsee, TSEECreateTextureFromPath(&tsee, "assets/parallax2.png"), 4);
	TSEECreateParallax(&tsee, TSEECreateTextureFromPath(&tsee, "assets/parallax3.png"), 2);
	TSEECreateParallax(&tsee, TSEECreateTextureFromPath(&tsee, "assets/parallax4.png"), 1);
	TSEEAddToolbarButton(&tsee, "_default", "Test");

	//TSEEInitNetworking(&tsee); // NOTE: COMING SOON!!

	while (tsee.window->running) {
		TSEEHandleEvents(&tsee);
		TSEECalculateDT(&tsee);
		TSEEPerformPhysics(&tsee);
		TSEERenderAll(&tsee);
	}

	TSEELog("Saving settings.\n");
	if (!TSEESaveSettings(&tsee)) {
		TSEEError("Failed to save settings. All changes will be lost.\nSorry.");
	}

	TSEELog("Closing.\n");

	TSEEClose(&tsee);
	return 0; 
}