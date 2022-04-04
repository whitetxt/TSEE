#include "../tsee.h"

/**
 * @brief Load a font into a TSEE object
 * 
 * @param tsee TSEE object to load the font into.
 * @param path Path to the font file.
 * @param size Font size
 * @param name Name of the font.
 * @return true on success, false on fail.
 */
bool TSEE_Font_Load(TSEE *tsee, char *path, int size, char *name) {
	TSEE_Font *font = xmalloc(sizeof(*font));
	font->font = TTF_OpenFont(path, size);
	if (font->font == NULL) {
		TSEE_Warn("Failed to load font `%s` (%s)\n", path, TTF_GetError());
		return false;
	}
	font->name = strdup(name);
	font->size = size;
	TSEE_Array_Append(tsee->fonts, font);
	return true;
}

/**
 * @brief Unloads a font from a TSEE object
 * 
 * @param tsee TSEE object to find the font in.
 * @param name Name of the font to unload
 * @return true on success, false on fail.
 */
bool TSEE_Font_Unload(TSEE *tsee, char *name) {
	for (size_t i = 0; i < tsee->fonts->size; i++) {
		TSEE_Font *font = tsee->fonts->data[i];
		if (strcmp(font->name, name) == 0) {
			TTF_CloseFont(font->font);
			xfree(font->name);
			xfree(font);
			TSEE_Array_Delete(tsee->fonts, i);
			return true;
		}
	}
	TSEE_Warn("Attempted to unload non-existant font `%s`\n", name);
	return false;
}

/**
 * @brief Unloads all fonts in a TSEE object.
 * 
 * @param tsee TSEE object to unload all fonts in.
 * @return true on success, false on fail.
 */
bool TSEE_Font_UnloadAll(TSEE *tsee) {
	for (size_t i = 0; i < tsee->fonts->size; i++) {
		TSEE_Font *font = TSEE_Array_Get(tsee->fonts, i);
		TTF_CloseFont(font->font);
		xfree(font->name);
		xfree(font);
	}
	TSEE_Array_Destroy(tsee->fonts);
	return true;
}

/**
 * @brief Gets an already loaded font from a TSEE object by its name.
 * 
 * @param tsee TSEE object to find the font in.
 * @param name Name of the font.
 * @return TTF_Font* 
 */
TTF_Font *TSEE_Font_Get(TSEE *tsee, char *name) {
	for (size_t i = 0; i < tsee->fonts->size; i++) {
		TSEE_Font *data = TSEE_Array_Get(tsee->fonts, i);
		if (strcmp(data->name, name) == 0) {
			return data->font;
		}
	}
	TSEE_Warn("Failed to find font `%s`\n", name);
	return NULL;
}