#define TSEE_DEV
#include "include/main.h"

void quitGame(void *t) {
	TSEE *tsee = (TSEE *)t;
	tsee->window->running = false;
}

void saveMap(void *t) {
	TSEE *tsee = (TSEE *)t;
	TSEESaveMap(tsee, "map.tsee_map");
}

void loadMap(void *t) {
	TSEE *tsee = (TSEE *)t;
	TSEELoadMap(tsee, "map.tsee_map");
}

void addTexture(void *t) {
	TSEE *tsee = (TSEE *)t;
	const char * filters[4] = {"*.jpg", "*.png", "*.tiff", "*.webp"};
	char *fn = tinyfd_openFileDialog("Open Texture Location", "", 4, filters, "Image files", false);
	TSEECreateTexture(tsee, fn);
	TSEELog("Loaded texture from `%s`\n", fn);
}

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;

	TSEE *tsee = TSEECreate(800, 600);

	if (!TSEEInitAll(tsee)) {
		TSEECritical("Failed to initialize TSEE\n");
		TSEEClose(tsee);
		return -1;
	}
	if (!TSEESetWindowTitle(tsee, "TSEE - Map Editor")) {
		TSEEWarn("Failed to set window title.\n");
	}

	TSEEAddToolbarButton(tsee, "_default", "Home");
	TSEEAddToolbarChild(tsee, "Home", "_default", "Quit", quitGame);
	TSEEAddToolbarButton(tsee, "_default", "Textures");
	TSEEAddToolbarChild(tsee, "Textures", "_default", "Add", addTexture);
	TSEEAddToolbarButton(tsee, "_default", "Maps");
	TSEEAddToolbarChild(tsee, "Maps", "_default", "Save", saveMap);
	TSEEAddToolbarChild(tsee, "Maps", "_default", "Load", loadMap);

	while (tsee->window->running) {
		TSEEHandleEvents(tsee);
		TSEECalculateDT(tsee);
		TSEERenderAll(tsee);
	}

	TSEELog("Saving settings.\n");
	if (!TSEESaveSettings(tsee)) {
		TSEEError("Failed to save settings. All changes will be lost.\nSorry.");
	}
	TSEELog("Closing.\n");
	TSEEClose(tsee);
	return 0; 
}