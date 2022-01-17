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

	int idx = TSEECreatePhysicsObject(&tsee, TSEECreateTexture(&tsee, "assets/test_image.png"), 1);
	TSEECreatePlayer(&tsee, TSEEArrayGet(tsee.world->physics_objects, idx));
	TSEESetPlayerJumpForce(&tsee, 30);
	TSEESetPlayerSpeed(&tsee, 50);
	TSEESetWorldGravity(&tsee, 15);
	TSEELog("Creating objects\n");
	for (size_t i = 0; i < 400; i++) {
		for (size_t j = 0; j < 100; j++) {
			idx = TSEECreateObject(&tsee, TSEECreateTexture(&tsee, "assets/test_image.png"));
			TSEESetObjectPosition(&tsee, idx, i, 595 - j);
		}
	}
	TSEECreateParallax(&tsee, TSEECreateTexture(&tsee, "assets/parallax1.png"), 6);
	TSEECreateParallax(&tsee, TSEECreateTexture(&tsee, "assets/parallax2.png"), 4);
	TSEECreateParallax(&tsee, TSEECreateTexture(&tsee, "assets/parallax3.png"), 2);
	TSEECreateParallax(&tsee, TSEECreateTexture(&tsee, "assets/parallax4.png"), 1);
	TSEEAddToolbarButton(&tsee, "_default", "Test");
	TSEEAddToolbarButton(&tsee, "_default", "Test2");
	TSEEAddToolbarButton(&tsee, "_default", "Test3");

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