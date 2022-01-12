#include "include/main.h"

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;
	TSEELog("Initialising TSEE Engine...\n");

	TSEE tsee = TSEECreate(800, 600);

	TSEELog("TSEE Engine initialised.\n");

	TSEELog("Initialising TSEE Components...\n");

	if (!TSEEInitAll(&tsee)) {
		TSEECritical("Failed to initialize TSEE\n");
		TSEEClose(&tsee);
		return -1;
	}

	if (!TSEESetWindowTitle(&tsee, "TSEE - Example Game")) {
		TSEEWarn("Failed to set window title.\n");
	}

	TSEELog("TSEE Components initialised.\n");

	int idx = TSEECreatePhysicsObject(&tsee, TSEECreateTextureFromPath(&tsee, "assets/test_image.png"), 1);
	TSEECreatePlayer(&tsee, TSEEArrayGet(tsee.world->physics_objects, idx));
	TSEESetPlayerJumpForce(&tsee, 30);
	TSEESetPlayerSpeed(&tsee, 50);
	TSEESetWorldGravity(&tsee, 15);
	idx = TSEECreateObject(&tsee, TSEECreateTextureFromPath(&tsee, "assets/test_image.png"));
	TSEESetObjectPosition(&tsee, idx, 150, 500);
	TSEECreateParallax(&tsee, TSEECreateTextureFromPath(&tsee, "assets/parallax1.png"), 6);
	TSEECreateParallax(&tsee, TSEECreateTextureFromPath(&tsee, "assets/parallax2.png"), 4);
	TSEECreateParallax(&tsee, TSEECreateTextureFromPath(&tsee, "assets/parallax3.png"), 2);
	TSEECreateParallax(&tsee, TSEECreateTextureFromPath(&tsee, "assets/parallax4.png"), 1);

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