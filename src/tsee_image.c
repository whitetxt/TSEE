#include "include/tsee.h"

TSEE_Texture *TSEECreateTextureFromPath(TSEE *tsee, char *path) {
	TSEE_Texture *texture = malloc(sizeof(*texture));
	if (!texture) {
		TSEEError("Couldn't malloc memory for texture \"%s\"\n", path);
		return NULL;
	}
	SDL_Texture *tex = IMG_LoadTexture(tsee->window->renderer, path);
	if (!tex) {
		TSEEError("Couldn't load texture from file \"%s\"\n", path);
		free(texture);
		return NULL;
	}
	texture->texture = tex;
	SDL_QueryTexture(tex, NULL, NULL, &texture->rect.w, &texture->rect.h);
	texture->rect.x = 0;
	texture->rect.y = 0;
	return texture;
}