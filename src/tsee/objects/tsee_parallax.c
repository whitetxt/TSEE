#include "../tsee.h"

bool TSEE_Parallax_Create(TSEE *tsee, TSEE_Texture *texture, int distanceFromCamera) {
	if (distanceFromCamera <= 0) {
		TSEE_Error("Distance from camera must be greater than 0 (Recieved %d)\n", distanceFromCamera);
		return false;
	}

	TSEE_Parallax *parallax = xmalloc(sizeof(*parallax));
	if (!parallax) {
		return false;
	}

	parallax->texture = texture;
	parallax->distance = distanceFromCamera;
	parallax->texture->rect.y = tsee->window->height - texture->rect.h;
	bool inserted = false;

	for (size_t j = 0; j < tsee->world->parallax->size; j++) {
		TSEE_Parallax *parallax2 = TSEE_Array_Get(tsee->world->parallax, j);
		if (parallax->distance > parallax2->distance) {
			TSEE_Array_Insert(tsee->world->parallax, parallax, j);
			inserted = true;
			break;
		}
	}

	if (!inserted) {
		TSEE_Array_Append(tsee->world->parallax, parallax);
	}
	return true;
}

bool TSEE_Parallax_CreateFromObject(TSEE *tsee, TSEE_Object *obj, int distanceFromCamera) {
	if (distanceFromCamera <= 0) {
		TSEE_Error("Distance from camera must be greater than 0 (Recieved %d)\n", distanceFromCamera);
		return false;
	}

	TSEE_Parallax *parallax = xmalloc(sizeof(*parallax));
	if (!parallax) {
		return false;
	}

	parallax->texture = obj->texture;
	parallax->distance = distanceFromCamera;
	parallax->texture->rect.y = tsee->window->height - obj->texture->rect.h;
	bool inserted = false;

	for (size_t j = 0; j < tsee->world->parallax->size; j++) {
		TSEE_Parallax *parallax2 = TSEE_Array_Get(tsee->world->parallax, j);
		if (parallax->distance > parallax2->distance) {
			TSEE_Array_Insert(tsee->world->parallax, parallax, j);
			inserted = true;
			break;
		}
	}

	if (!inserted) {
		TSEE_Array_Append(tsee->world->parallax, parallax);
	}
	return true;
}

bool TSEE_Parallax_Render(TSEE *tsee) {
	for (size_t i = 0; i < tsee->world->parallax->size; i++) {
		TSEE_Parallax *parallax = TSEE_Array_Get(tsee->world->parallax, i);
		SDL_Rect newRect = parallax->texture->rect;
		newRect.x = tsee->world->scroll_x * (-1 / parallax->distance);
		while (newRect.x > tsee->window->width) {
			newRect.x -= tsee->window->width;
		}
		while (newRect.x + newRect.w < 0) {
			newRect.x += tsee->window->width;
		}
		SDL_RenderCopy(	tsee->window->renderer,
						parallax->texture->texture,
						NULL,
						&newRect);
		while (newRect.x + newRect.w < tsee->window->width) {
			newRect.x += tsee->window->width;
			SDL_RenderCopy(	tsee->window->renderer,
							parallax->texture->texture,
							NULL,
							&newRect);
		}
		// Render parallax again to left or right depending on scroll
		while (newRect.x > 0) {
			newRect.x -= newRect.w;
			SDL_RenderCopy(	tsee->window->renderer,
							parallax->texture->texture,
							NULL,
							&newRect);
		}
		while (newRect.x < 0) {
			newRect.x += newRect.w;
			SDL_RenderCopy(	tsee->window->renderer,
							parallax->texture->texture,
							NULL,
							&newRect);
		}
	}
	return true;
}

void TSEE_Parallax_Destroy(TSEE_Parallax *para, bool destroyTexture) {
	if (destroyTexture) {
		TSEE_Texture_Destroy(para->texture);
	}
	xfree(para);
}