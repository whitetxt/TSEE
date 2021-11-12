#define ZED_NET_IMPLEMENTATION
#include <zed_net.h>
#include "include/main.h"

void render() {
	// Main render function. renders the player, all the tiles and all text
}

void calculateDT() {
	// Calculates delta time
}

void calculateFPS() {
	// Calculates the FPS from deltatime.
	if (dt != 0) {
		// Before I turned vsync on, dt was sometimes 0, therefore crashing when divide by 0.
		double EstFPS = 1000 / (dt * 1000);
		printf("FPS: %f - %fms taken.\n", EstFPS, dt * 1000);
	} else {
		printf("FPS: INF - 0ms taken.\n");
	}
	return;
}

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;
	printf("Started.\n");

	loadSettings();
	TSEE tsee = createTSEEGame("TSEE - Example Game", 800, 600);
	if (initTSEERendering() != true) {
		printf("Failed to initialize TSEE Rendering Module.\n");
		return -1;
	}
	initTSEEInput(tsee);
	initTSEEText(tsee);
	initTSEENetwork(tsee);
	initTSEEEvents(tsee);

	printf("Saving settings.\n");
	if (saveSettings() != 0) {
		printf("Failed to save settings. All changes will be lost.\nSorry.");
	}

	printf("Closing.\n");

	TSEE_Close(tsee);
	return 0; 
}

