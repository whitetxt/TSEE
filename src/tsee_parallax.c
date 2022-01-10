#include "include/tsee.h"

bool TSEECreateParallax(TSEE *tsee, TSEE_Texture *texture, int distanceFromCamera) {
	if (distanceFromCamera < 0) {
		TSEEError("Distance from camera must be greater than 0 (Recieved %d)\n", distanceFromCamera);
		return false;
	}
	TSEE_Parallax *parallax = malloc(sizeof(TSEE_Parallax));
	if (parallax == NULL) {
		return false;
	}
	parallax->texture = texture;
	parallax->distance = distanceFromCamera;
	parallax->texture->rect.y = tsee->window->height - texture->rect.h;
	TSEEArrayAppend(tsee->world->parallax, parallax);
	return true;
}

bool TSEERenderParallax(TSEE *tsee) {
	for (size_t i = 0; i < tsee->world->parallax->size; i++) {
		TSEE_Parallax *parallax = TSEEArrayGet(tsee->world->parallax, i);
		SDL_Rect newRect = parallax->texture->rect;
		newRect.x = tsee->world->scroll_x * (1 / parallax->distance);
		SDL_RenderCopy(	tsee->window->renderer,
						parallax->texture->texture,
						NULL,
						&newRect);
	}
	return true;
}