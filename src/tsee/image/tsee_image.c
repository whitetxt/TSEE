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
	TSEE_Image *img = NULL;
	if (img = TSEE_Resource_Image_Get(tsee, path)) {
		TSEE_Texture *newTexture = xmalloc(sizeof(*newTexture));
		if (!newTexture) {
			TSEE_Error("Couldn't malloc memory for texture `%s`\n", path);
			return NULL;
		}
		newTexture->image = img;
		SDL_QueryTexture(img->texture, NULL, NULL, &newTexture->rect.w, &newTexture->rect.h);
		newTexture->rect = (SDL_Rect){0, 0, newTexture->rect.w, newTexture->rect.h};
		TSEE_Resource_Texture_Store(tsee, newTexture);
		return newTexture;
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
	TSEE_Image *img = xmalloc(sizeof(*img));
	if (!img) {
		TSEE_Error("Couldn't malloc for image `%s`\n", path);
		xfree(tex);
		xfree(texture);
		return NULL;
	}
	img->texture = tex;
	img->path = strdup(path);
	TSEE_Resource_Image_Store(tsee, img);

	SDL_QueryTexture(tex, NULL, NULL, &texture->rect.w, &texture->rect.h);
	texture->image = img;
	texture->rect = (SDL_Rect){0, 0, texture->rect.w, texture->rect.h};
	TSEE_Resource_Texture_Store(tsee, texture);
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
 * @deprecated
 * 
 * @param tex Texture to destroy
 */
void TSEE_Texture_Destroy(TSEE *tsee, TSEE_Texture *tex) {
	if (!tex) return;
	if (tex->texture)
		SDL_DestroyTexture(tex->texture);
	if (tex->path)
		xfree(tex->path);
	for (size_t i = 0; i < tsee->textures->size; i++) {
		if (tex == TSEE_Array_Get(tsee->textures, i)) {
			TSEE_Array_Delete(tsee->textures, i);
			break;
		}
	}
	xfree(tex);
}