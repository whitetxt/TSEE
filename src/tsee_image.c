#include "include/tsee.h"

TSEE_Texture *TSEECreateTexture(TSEE *tsee, char *path) {
	for (size_t i = 0; i < tsee->textures->size; i++) {
		TSEE_Texture *existingTexture = (TSEE_Texture *)TSEEArrayGet(tsee->textures, i);
		if (strcmp(existingTexture->path, path) == 0) {
			TSEE_Texture *newTexture = malloc(sizeof(*newTexture));
			newTexture->texture = existingTexture->texture;
			SDL_QueryTexture(newTexture->texture, NULL, NULL, &newTexture->rect.w, &newTexture->rect.h);
			newTexture->rect = (SDL_Rect){0, 0, newTexture->rect.w, newTexture->rect.h};
			newTexture->path = strdup(path);
			TSEEArrayAppend(tsee->texture_copies, newTexture);
			return newTexture;
		}
	}
	TSEE_Texture *texture = malloc(sizeof(*texture));
	if (!texture) {
		TSEEError("Couldn't malloc memory for texture `%s`\n", path);
		return NULL;
	}
	SDL_Texture *tex = IMG_LoadTexture(tsee->window->renderer, path);
	if (!tex) {
		TSEEError("Couldn't load texture from file `%s`\n", path);
		free(texture);
		return NULL;
	}
	texture->texture = tex;
	SDL_QueryTexture(tex, NULL, NULL, &texture->rect.w, &texture->rect.h);
	texture->rect.x = 0;
	texture->rect.y = 0;
	texture->path = strdup(path);
	TSEEArrayAppend(tsee->textures, texture);
	return texture;
}

TSEE_Texture *TSEEFindTexture(TSEE *tsee, char *path) {
	for (size_t i = 0; i < tsee->textures->size; i++) {
		TSEE_Texture *texture = (TSEE_Texture *)TSEEArrayGet(tsee->textures, i);
		TSEELog("Checking texture %s against %s\n", texture->path, path);
		if (strcmp(texture->path, path) == 0) {
			TSEELog("Found texture with same path %s\n", path);
			return texture;
		}
	}
	TSEEWarn("Couldn't find loaded texture `%s`\n", path);
	return NULL;
}

void TSEEDestroyTexture(TSEE_Texture *tex) {
	if (!tex) {
		return;
	}
	SDL_DestroyTexture(tex->texture);
	free(tex->path);
	free(tex);
}