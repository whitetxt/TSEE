#include "../tsee.h"

/**
 * @brief Load a font into a TSEE object
 *
 * @param tsee TSEE object to load the font into.
 * @param path Path to the font file.
 * @param size Font size
 * @param name Name of the font.
 * @return success status
 */
bool TSEE_Font_Load(TSEE *tsee, char *path, int size, char *name) {
	TSEE_Font *font = xmalloc(sizeof(*font));
	if (!font) {
		TSEE_Error("Failed to malloc for font.\n");
		return false;
	}
	font->font = TTF_OpenFont(path, size);
	if (font->font == NULL) {
		TSEE_Warn("Failed to load font `%s` (%s)\n", path, TTF_GetError());
		return false;
	}
	font->name = strdup(name);
	font->size = size;
	return TSEE_Resource_Font_Store(tsee, font);
}

/**
 * @brief Unloads a font by its pointer.
 *
 * @param font Pointer to the font to unload.
 * @return success status
 */
bool TSEE_Font_Unload(TSEE *tsee, TSEE_Font *font) {
	if (!font) {
		TSEE_Warn("Attempted to unload NULL font\n");
		return false;
	}
	TTF_CloseFont(font->font);
	xfree(font->name);
	xfree(font);
	return TSEE_Resource_Font_Delete(tsee, font);
}

/**
 * @brief Unloads a font from a TSEE object identified by its name
 *
 * @param name Name of the font to unload
 * @return success status
 */
bool TSEE_Font_UnloadName(TSEE *tsee, char *name) {
	TSEE_Font *font = TSEE_Resource_Font_Get(tsee, name);
	if (!font) {
		TSEE_Warn("Attempted to unload non-existant font `%s`\n", name);
		return false;
	}
	return TSEE_Font_Unload(tsee, font);
}

/**
 * @brief Unloads all fonts in a TSEE object.
 *
 * @param tsee TSEE object to unload all fonts in.
 * @return success status
 */
bool TSEE_Font_UnloadAll(TSEE *tsee) {
	while (tsee->resources->fonts->size > 0) {
		TSEE_Font *font = TSEE_Array_Get(tsee->resources->fonts, 0);
		TSEE_Font_Unload(tsee, font);
	}
	return true;
}

/**
 * @brief Gets an already loaded font from a TSEE object by its name.
 *
 * @param tsee TSEE object to find the font in.
 * @param name Name of the font.
 * @return TTF_Font*
 */
TSEE_Font *TSEE_Font_Get(TSEE *tsee, char *name) {
	if (!name) {
		TSEE_Warn("Attempted to get font with no name.\n");
		return NULL;
	}
	TSEE_Font *font = TSEE_Resource_Font_Get(tsee, name);
	if (!font) {
		TSEE_Warn("Failed to find font `%s`\n", name);
		return NULL;
	}
	return font;
}