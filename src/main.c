#include "include/main.h"

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;
	TSEELog("Initialising TSEE Engine...\n");

	TSEE tsee = TSEECreate(800, 600);
	TSEELoadSettings(&tsee);

	TSEELog("TSEE Engine initialised.\n");

	TSEELog("Initialising TSEE Components...\n");

	if (TSEEInitRendering(&tsee) != true) {
		TSEECritical("Failed to initialize TSEE Rendering Module.\n");
		return -1;
	}

	if (TSEESetWindowTitle(&tsee, "TSEE - Example Game") != true) {
		TSEEWarn("Failed to set window title.\n");
	}

	if (TSEEInitText(&tsee, true) != true) {
		TSEECritical("Failed to initialize TSEE Text Module.\n");
		TSEEClose(&tsee);
		return -1;
	}

	TSEELog("TSEE Components initialised.\n");

	int idx = TSEECreatePhysicsObject(&tsee, TSEECreateTextureFromPath(&tsee, "assets/test_image.png"), 1);
	TSEECreatePlayer(&tsee, TSEEArrayGet(tsee.world->physics_objects, idx));
	TSEESetPlayerJumpForce(&tsee, 30);
	TSEESetPlayerSpeed(&tsee, 50);
	TSEESetWorldGravity(&tsee, 15);

	TSEEInitEvents(&tsee);
	TSEEInitInput(&tsee);
	//TSEEInitNetworking(&tsee); // NOTE: COMING SOON!!

	while (tsee.window->running) {
		//TSEEApplyFriction(&tsee);
		TSEEHandleEvents(&tsee);
		TSEECalculateDT(&tsee);
		TSEEPerformPhysics(&tsee);
		TSEERenderAll(&tsee);
	}

	TSEELog("Saving settings.\n");
	if (TSEESaveSettings(&tsee) != true) {
		TSEEError("Failed to save settings. All changes will be lost.\nSorry.");
	}

	TSEELog("Closing.\n");

	TSEEClose(&tsee);
	return 0; 
}