#include "../tsee.h"

/**
 * @brief Creates a texture from a file path. If the texture is already loaded,
 *        the new texture uses a reference to the other texture.
 *
 * @param path Path to read the texture from.
 * @return TSEE_Texture*
 */
TSEE_Texture *TSEE_Texture_Create(TSEE *tsee, char *path) {
	TSEE_Texture *tex = NULL;
	if ((tex = TSEE_Resource_Texture_Get(tsee, path))) {
		TSEE_Texture *newTexture = xmalloc(sizeof(*newTexture));
		if (!newTexture) {
			TSEE_Error("Couldn't malloc memory for texture `%s`\n", path);
			return NULL;
		}
		newTexture->texture = tex->texture;
		SDL_QueryTexture(tex->texture, NULL, NULL, &newTexture->rect.w,
						 &newTexture->rect.h);
		newTexture->rect =
			(SDL_Rect){0, 0, newTexture->rect.w, newTexture->rect.h};
		newTexture->path = strdup(path);
		TSEE_Resource_Texture_Store(tsee, newTexture);
		return newTexture;
	}
	SDL_Texture *SDL_tex = IMG_LoadTexture(tsee->window->renderer, path);
	if (!SDL_tex) {
		TSEE_Error("Couldn't load texture from file `%s`\n", path);
		return NULL;
	}
	tex = xmalloc(sizeof(*tex));
	if (!tex) {
		TSEE_Error("Couldn't malloc for texture `%s`\n", path);
		xfree(SDL_tex);
		return NULL;
	}
	tex->texture = SDL_tex;
	tex->path = strdup(path);

	SDL_QueryTexture(SDL_tex, NULL, NULL, &tex->rect.w, &tex->rect.h);
	tex->rect = (SDL_Rect){0, 0, tex->rect.w, tex->rect.h};
	TSEE_Resource_Texture_Store(tsee, tex);
	return tex;
}

/**
 * @brief Finds a texture if it is already loaded.
 *
 * @param path Path to the texture
 * @return TSEE_Texture*
 */
TSEE_Texture *TSEE_Texture_Find(TSEE *tsee, char *path) {
	TSEE_Texture *texture = TSEE_Resource_Texture_Get(tsee, path);
	if (!texture) {
		TSEE_Warn("Couldn't find texture `%s`\n", path);
		return NULL;
	}
	return texture;
}

/**
 * @brief Destroys a texture
 *
 * @param tex Texture to destroy
 */
void TSEE_Texture_Destroy(TSEE_Texture *tex) {
	if (!tex)
		return;
	if (tex->path) {
		xfree(tex->path);
	}
	if (tex->texture) {
		SDL_DestroyTexture(tex->texture);
	}
	xfree(tex);
}