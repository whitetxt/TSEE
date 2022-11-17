#include "../tsee.h"

bool TSEE_Resource_Init(TSEE *tsee) {
	tsee->resources = xmalloc(sizeof(*tsee->resources));
	if (!tsee->resources) {
		TSEE_Error("Failed to malloc for resources.\n");
		return false;
	}
	tsee->resources->textures = TSEE_Array_Create();
	tsee->resources->fonts = TSEE_Array_Create();
	return true;
}

/**
 * @brief Unloads and frees all data in the resource manager.
 * 
 * @return success status
 */
bool TSEE_Resource_Unload(TSEE *tsee) {
	if (!tsee->resources) return false;
	while (tsee->resources->textures->size > 0) {
		TSEE_Texture *tex = TSEE_Array_Get(tsee->resources->textures, 0);
		TSEE_Texture_Destroy(tex);
		TSEE_Array_Delete(tsee->resources->textures, 0);
	}
	TSEE_Array_Destroy(tsee->resources->textures);
	tsee->resources->textures = TSEE_Array_Create();
	while (tsee->resources->fonts->size > 0) {
		TSEE_Font *font = TSEE_Array_Get(tsee->resources->fonts, 0);
		TSEE_Font_Unload(tsee, font);
	}
	TSEE_Array_Destroy(tsee->resources->fonts);
	tsee->resources->fonts = TSEE_Array_Create();
	return true;
}

/**
 * @brief Gets a texture from the resource manager.
 * 
 * @param path Path of the texture to get.
 * @return TSEE_Texture* on success, NULL on failure.
 */
TSEE_Texture *TSEE_Resource_Texture_Get(TSEE *tsee, char *path) {
	for (size_t i = 0; i < tsee->resources->textures->size; i++) {
		TSEE_Texture *tex = TSEE_Array_Get(tsee->resources->textures, i);
		if (strcmp(path, tex->path) == 0) {
			return tex;
		}
	}
	return NULL;
}

/**
 * @brief Stores a texture in the resource manager.
 * 
 * @param tex Texture to store 
 * @return success status
 */
bool TSEE_Resource_Texture_Store(TSEE *tsee, TSEE_Texture *tex) {
	return TSEE_Array_Append(tsee->resources->textures, tex);
}

/**
 * @brief Deletes a texture in the resource manager.
 * !! It is up to the calling function to ensure that memory is freed.
 * 
 * @param tex Texture to delete.
 * @return success status
 */
bool TSEE_Resource_Texture_Delete(TSEE *tsee, TSEE_Texture *tex) {
	return TSEE_Array_DeleteItem(tsee->resources->textures, tex);
}

/**
 * @brief Deletes a texture in the resource manager by its path.
 * 
 * @param path Path of the texture to delete.
 * @return success status
 */
bool TSEE_Resource_Texture_DeletePath(TSEE *tsee, char *path) {
	TSEE_Texture *tex = TSEE_Resource_Texture_Get(tsee, path);
	if (!tex) {
		TSEE_Warn("Attempted to delete non-existant resource `%s`\n", path);
		return false;
	}
	int idx = TSEE_Array_GetIndex(tsee->resources->textures, tex);
	if (idx == -1) {
		TSEE_Warn("Attempted to delete non-existant resource `%s`\n", path);
		return false;
	}
	TSEE_Array_Delete(tsee->resources->textures, idx);
	return true;
}

/**
 * @brief Gets a font from TSEE's resource manager.
 * 
 * @param name Name of the font to find.
 * @return TSEE_Font* on success, NULL on fail.
 */
TSEE_Font *TSEE_Resource_Font_Get(TSEE *tsee, char *name) {
	for (size_t i = 0; i < tsee->resources->fonts->size; i++) {
		TSEE_Font *foundFont = TSEE_Array_Get(tsee->resources->fonts, i);
		if (strcmp(name, foundFont->name) == 0) {
			return foundFont;
		}
	}
	return NULL;
}

/**
 * @brief Stores a font in TSEE's resource manager.
 * Will warn if storing a duplicate font.
 * 
 * @param font Font to store
 * @return if the operation succeeded.
 */
bool TSEE_Resource_Font_Store(TSEE *tsee, TSEE_Font *font) {
	if (TSEE_Resource_Font_Get(tsee, font->name) == font) {
		TSEE_Warn("Storing duplicate font: `%s`\n", font->name);
	}
	return TSEE_Array_Append(tsee->resources->fonts, font);
}

/**
 * @brief Deletes a font from TSEE's resource manager.
 * 
 * @param font Font to delete.
 * @return if the operation succeeded.
 */
bool TSEE_Resource_Font_Delete(TSEE *tsee, TSEE_Font *font) {
	return TSEE_Array_DeleteItem(tsee->resources->fonts, font);
}