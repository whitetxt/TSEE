#include "include/tsee.h"

bool TSEEInitText(TSEE *tsee, bool loadDefault) {
	tsee->fonts = TSEEArrayCreate();
	if (loadDefault)
		TSEELoadFont(tsee, "fonts/personn.ttf", 16, "_default");
	return true;
}

bool TSEELoadFont(TSEE *tsee, char *path, int size, char *name) {
	TSEE_Font *font = malloc(sizeof(*font));
	font->font = TTF_OpenFont(path, size);
	if (font->font == NULL) {
		TSEEWarn("Failed to load font: %s\n", path);
		return false;
	}
	font->name = strdup(name);
	font->size = size;
	TSEEArrayAppend(tsee->fonts, font);
	return true;
}

bool TSEEUnloadFont(TSEE *tsee, char *name) {
	for (size_t i = 0; i < tsee->fonts->size; i++) {
		TSEE_Font *font = tsee->fonts->data[i];
		if (strcmp(font->name, name) == 0) {
			TTF_CloseFont(font->font);
			free(font->name);
			free(font);
			TSEEArrayDelete(tsee->fonts, i);
			return true;
		}
	}
	TSEEWarn("Attempted to unload non-existant font: %s\n", name);
	return false;
}

bool TSEEUnloadAllFonts(TSEE *tsee) {
	for (size_t i = 0; i < tsee->fonts->size; i++) {
		TSEE_Font *font = tsee->fonts->data[i];
		TTF_CloseFont(font->font);
		free(font->name);
	}
	TSEEArrayClear(tsee->fonts);
	return true;
}

TTF_Font *TSEEGetFont(TSEE *tsee, char *name) {
	for (size_t i = 0; i < tsee->fonts->size; i++) {
		TSEE_Font *data = tsee->fonts->data[i];
		if (strcmp(data->name, name) == 0) {
			return data->font;
		}
	}
	TSEEWarn("Failed to find font: %s\n", name);
	return NULL;
}