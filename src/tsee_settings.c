#include "include/tsee.h"

void TSEESettingsLoadCallback(TSEE *tsee, char *section, char *value) {
	if (strcmp(section, "width") == 0) {
		tsee->window->width = atoi(value);
		TSEEWindowUpdateSize(tsee);
	} else if (strcmp(section, "height") == 0) {
		tsee->window->height = atoi(value);
		TSEEWindowUpdateSize(tsee);
	} else {
		TSEEWarn("Unknown settings section \"%s\" with value \"%s\"\n", section, value);
	}
}

bool TSEELoadSettings(TSEE *tsee) {
	FILE *fp = fopen("TSEE-Settings.ini", "r");
	if (!fp) {
		TSEEError("Couldn't open settings file \"TSEE-Settings.ini\"\n");
		return false;
	}
	char buf[512];
	while (fgets(buf, 512, fp)) {
		char *sec, *val;
		sec = strtok(buf, "=");
		val = strtok(NULL, "=");
		TSEESettingsLoadCallback(tsee, sec, val);
	}
	return true;
}

bool TSEESaveSettings(TSEE *tsee) {
	FILE *fp = fopen("TSEE-Settings.ini", "w");
	if (!fp) {
		TSEEError("Couldn't open settings file \"TSEE-Settings.ini\"\nYour settings have been lost.");
		return false;
	}
	fprintf(fp, "width=%d\n", tsee->window->width);
	fprintf(fp, "height=%d\n", tsee->window->height);
	fclose(fp);
	return true;
}