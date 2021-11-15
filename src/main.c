#include "include/main.h"

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;
	printf("Started.\n");

	loadSettings();
	TSEE tsee = TSEECreate(800, 600);
	if (TSEEInitRendering(&tsee) != true) {
		TSEECritical("Failed to initialize TSEE Rendering Module.\n");
		return -1;
	}
	if (TSEESetWindowTitle(&tsee, "TSEE - Example Game") != true) {
		TSEEWarn("Failed to set window title.\n");
	}
	if (TSEEInitText(&tsee) != true) {
		TSEECritical("Failed to initialize TSEE Text Module.\n");
		TSEEClose(&tsee);
		return -1;
	}
	TSEEInitInput(tsee);
	TSEEInitEvents(tsee);
	//TSEEInitNetworking(tsee); // NOTE: COMING SOON!!

	printf("Saving settings.\n");
	if (saveSettings() != 0) {
		printf("Failed to save settings. All changes will be lost.\nSorry.");
	}

	printf("Closing.\n");

	TSEE_Close(tsee);
	return 0; 
}

