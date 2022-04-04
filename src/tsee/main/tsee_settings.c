#include "../tsee.h"

void TSEE_Settings_LoadCallback(TSEE *tsee, char *section, char *value) {
	if (strcmp(section, "width") == 0) {
		tsee->window->width = atoi(value);
		TSEE_Window_UpdateSize(tsee);
	} else if (strcmp(section, "height") == 0) {
		tsee->window->height = atoi(value);
		TSEE_Window_UpdateSize(tsee);
	} else {
		TSEE_Warn("Unknown settings section `%s` with value `%s`\n", section, value);
	}
}

bool TSEE_Settings_Load(TSEE *tsee) {
	FILE *fp = fopen("TSEE-Settings.ini", "r");
	if (!fp) {
		TSEE_Error("Couldn't open settings file `TSEE-Settings.ini`\n");
		return false;
	}
	char buf[512];
	while (fgets(buf, 512, fp)) {
		char *sec, *val;
		sec = strtok(buf, "=");
		val = strtok(NULL, "=");
		TSEE_Settings_LoadCallback(tsee, sec, val);
	}
	return true;
}

bool TSEE_Settings_Save(TSEE *tsee) {
	FILE *fp = fopen("TSEE-Settings.ini", "w");
	if (!fp) {
		TSEE_Error("Couldn't open settings file `TSEE-Settings.ini`\nYour settings have been lost.");
		return false;
	}
	fprintf(fp, "width=%d\n", tsee->window->width);
	fprintf(fp, "height=%d\n", tsee->window->height);
	fclose(fp);
	return true;
}