#include "../tsee.h"

/**
 * @brief Creates a texture from a file path. If the texture is already loaded,
 *        the new texture uses a reference to the other texture.
 * 
 * @param tsee TSEE object to load the texture into
 * @param path Path to read the texture from.
 * @return TSEE_Texture* 
 */
TSEE_Texture *TSEE_Texture_Create(TSEE *tsee, char *path) {
	for (size_t i = 0; i < tsee->textures->size; i++) {
		TSEE_Texture *existingTexture = (TSEE_Texture *)TSEE_Array_Get(tsee->textures, i);
		if (strcmp(existingTexture->path, path) == 0) {
			TSEE_Texture *newTexture = xmalloc(sizeof(*newTexture));
			newTexture->texture = existingTexture->texture;
			SDL_QueryTexture(newTexture->texture, NULL, NULL, &newTexture->rect.w, &newTexture->rect.h);
			newTexture->rect = (SDL_Rect){0, 0, newTexture->rect.w, newTexture->rect.h};
			newTexture->path = strdup(path);
			TSEE_Array_Append(tsee->textures, newTexture);
			return newTexture;
		}
	}
	TSEE_Texture *texture = xmalloc(sizeof(*texture));
	if (!texture) {
		TSEE_Error("Couldn't malloc memory for texture `%s`\n", path);
		return NULL;
	}
	SDL_Texture *tex = IMG_LoadTexture(tsee->window->renderer, path);
	if (!tex) {
		TSEE_Error("Couldn't load texture from file `%s`\n", path);
		xfree(texture);
		return NULL;
	}
	texture->texture = tex;
	SDL_QueryTexture(tex, NULL, NULL, &texture->rect.w, &texture->rect.h);
	texture->rect.x = 0;
	texture->rect.y = 0;
	texture->path = strdup(path);
	TSEE_Array_Append(tsee->textures, texture);
	return texture;
}

/**
 * @brief Finds a texture if it is already loaded.
 * 
 * @param tsee TSEE object to look for the texture in
 * @param path Path to the texture
 * @return TSEE_Texture* 
 */
TSEE_Texture *TSEE_Texture_Find(TSEE *tsee, char *path) {
	for (size_t i = 0; i < tsee->textures->size; i++) {
		TSEE_Texture *texture = (TSEE_Texture *)TSEE_Array_Get(tsee->textures, i);
		TSEE_Log("Checking texture %s against %s\n", texture->path, path);
		if (strcmp(texture->path, path) == 0) {
			TSEE_Log("Found texture with same path %s\n", path);
			return texture;
		}
	}
	TSEE_Warn("Couldn't find loaded texture `%s`\n", path);
	return NULL;
}

/**
 * @brief Destroys a texture
 * 
 * @param tex Texture to destroy
 */
void TSEE_Texture_Destroy(TSEE_Texture *tex) {
	if (!tex) return;
	if (tex->texture)
		SDL_DestroyTexture(tex->texture);
	xfree(tex->path);
	xfree(tex);
}