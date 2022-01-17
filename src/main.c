#include "include/main.h"

void quitGame(void *t) {
	TSEE *tsee = (TSEE *)t;
	tsee->window->running = false;
}

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
	idx = TSEECreateObject(&tsee, TSEECreateTexture(&tsee, "assets/test_image.png"));
	TSEESetObjectPosition(&tsee, idx, 160, 595);
	idx = TSEECreateObject(&tsee, TSEECreateTexture(&tsee, "assets/test_image.png"));
	TSEESetObjectPosition(&tsee, idx, 170, 590);
	idx = TSEECreateObject(&tsee, TSEECreateTexture(&tsee, "assets/test_image.png"));
	TSEESetObjectPosition(&tsee, idx, 180, 585);
	TSEECreateParallax(&tsee, TSEECreateTexture(&tsee, "assets/parallax1.png"), 6);
	TSEECreateParallax(&tsee, TSEECreateTexture(&tsee, "assets/parallax2.png"), 4);
	TSEECreateParallax(&tsee, TSEECreateTexture(&tsee, "assets/parallax3.png"), 2);
	TSEECreateParallax(&tsee, TSEECreateTexture(&tsee, "assets/parallax4.png"), 1);
	TSEEAddToolbarButton(&tsee, "_default", "Test");
	TSEEAddToolbarButton(&tsee, "_default", "Test2");
	TSEEAddToolbarButton(&tsee, "_default", "Test3");
	TSEEAddToolbarChild(&tsee, TSEEArrayGet(tsee.ui->toolbar, 0), "_default", "Quit", quitGame);

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