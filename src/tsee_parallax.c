#include "include/tsee.h"

bool TSEECreateParallax(TSEE *tsee, TSEE_Texture *texture, int distanceFromCamera) {
	TSEE_Parallax *parallax = malloc(sizeof(TSEE_Parallax));
	if (parallax == NULL) {
		return false;
	}
	parallax->texture = texture;
	parallax->distance = distanceFromCamera;
	TSEEArrayAppend(tsee->world->parallax, parallax);
}

bool TSEERenderParallax(TSEE *tsee) {
	TSEE_Array *renderOrder = TSEEArrayCreate();
	if (renderOrder == NULL) {
		return false;
	}
	for (size_t i = 0; i < tsee->world->parallax->size; i++) {
		int idx = 0;
		while (((TSEE_Parallax *)TSEEArrayGet(renderOrder, idx))->distance < ((TSEE_Parallax *)TSEEArrayGet(tsee->world->parallax, i))->distance) {
			idx++;
		}
		TSEEArrayInsert(renderOrder, TSEEArrayGet(tsee->world->parallax, i), idx);
	}
	for (size_t i = 0; i < renderOrder->size; i++) {
		SDL_RenderCopy(	tsee->window->renderer,
						((TSEE_Parallax *)TSEEArrayGet(renderOrder, i))->texture->texture,
						NULL,
						&((TSEE_Parallax *)TSEEArrayGet(renderOrder, i))->texture->rect);
	}
}