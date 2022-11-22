#include "../tsee.h"

/**
 * @brief Callback for TSEE_Settings_Load, DO NOT USE.
 *
 * @param tsee TSEE object to load settings for
 * @param section Section which the setting is in
 * @param value Value for the setting
 */
void TSEE_Settings_LoadCallback(TSEE *tsee, char *section, char *value) {
	while (value[strlen(value) - 1] == '\n' ||
		   value[strlen(value) - 1] ==
			   ' ') {  // Remove trailing newlines and spaces
		value[strlen(value) - 1] = '\0';
	}
	if (section[0] == '#')
		return;	 // Ignore comments
	if (strcmp(section, "width") == 0) {
		tsee->window->width = atoi(value);
		TSEE_Window_UpdateSize(tsee);
	} else if (strcmp(section, "height") == 0) {
		tsee->window->height = atoi(value);
		TSEE_Window_UpdateSize(tsee);
	} else if (strcmp(section, "vsync") == 0) {
		bool result = false;
		bool success = TSEE_Settings_ConvertToBool(value, &result);
		if (!success) {
			TSEE_Warn("Invalid boolean value: `%s` (Found in setting `%s`)\n",
					  value, section);
		} else {
			tsee->window->vsync = result;
			TSEE_Warn(
				"Unable to change VSync settings (Old SDL version is "
				"incompatable)\n");
			// TSEE_Window_UpdateVsync(tsee);
		}
	} else {
		TSEE_Warn("Unknown settings section `%s` with value `%s`\n", section,
				  value);
	}
}

/**
 * @brief Loads TSEE's internal settings.
 *
 * @param tsee TSEE object to load into.
 * @return success status
 */
bool TSEE_Settings_Load(TSEE *tsee) {
	FILE *fp = fopen("TSEE-Settings.ini", "r");
	if (!fp) {
		TSEE_Warn(
			"Couldn't open settings file `TSEE-Settings.ini`\nCreating basic "
			"settings file...\n");
		TSEE_Settings_Save(tsee);
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

/**
 * @brief Saves TSEE's internal settings.
 *
 * @param tsee TSEE object to save.
 * @return success status
 */
bool TSEE_Settings_Save(TSEE *tsee) {
	FILE *fp = fopen("TSEE-Settings.ini", "w");
	if (!fp) {
		TSEE_Error(
			"Couldn't open settings file `TSEE-Settings.ini`\nYour settings "
			"have been lost.");
		return false;
	}
	fprintf(fp, "width=%d\n", tsee->window->width);
	fprintf(fp, "height=%d\n", tsee->window->height);
	fclose(fp);
	return true;
}

/**
 * @brief Converts a string value to a boolean
 *
 * @param value Value to convert.
 * @param result Boolean to store the result in.
 * @return true if it succeeds, false if the value is invalid.
 */
bool TSEE_Settings_ConvertToBool(char *value, bool *result) {
	if (strcmp(value, "true") == 0) {
		*result = true;
		return true;
	} else if (strcmp(value, "false") == 0) {
		*result = false;
		return true;
	}
	return false;
}