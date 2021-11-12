#include "include/main.h"
#include "include/ini.h"

configuration Settings;

static int handler(void* user, const char* section, const char* name, const char* value) { // Stolen straight from an example lol
	configuration* pconfig = (configuration*)user;

	#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
	if (MATCH("multiplayer", "name")) {
		pconfig->name = strdup(value);
	} else if (MATCH("multiplayer", "playertextsize")) {
		pconfig->playerTextSize = atoi(value);
	} else if (MATCH("video", "width")) {
		pconfig->width = atoi(value);
	} else if (MATCH("video", "height")) {
		pconfig->height = atoi(value);
	} else {
		return 0;  /* unknown section/name, error */
	}
	return 1;
}

// Loads settings from "settings.ini"
// @return 0 on success, 1 on failure
int loadSettings() {
	if (ini_parse("settings.ini", handler, &Settings) < 0) {
		printf("Can't load 'settings.ini'\n");
		return 1;
	}
	return 0;
}

// Saves settings to "settings.ini"
// @return 0 on success, 1 on failure
int saveSettings() {
	FILE* fp = fopen("settings.ini", "w");
	if (!fp) {
		printf("Can't open 'settings.ini'\n");
		return 1;
	}
	fprintf(fp, "[video]\n");
	fprintf(fp, "width=%d\n", Settings.width);
	fprintf(fp, "height=%d\n", Settings.height);
	fprintf(fp, "\n[multiplayer]\n");
	fprintf(fp, "name=%s\n", Settings.name);
	fprintf(fp, "playertextsize=%d\n", Settings.playerTextSize);
	fclose(fp);
	return 0;
}