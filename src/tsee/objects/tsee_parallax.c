#include "../tsee.h"

/**
 * @brief Creates a parallax object from a texture.
 * 
 * @param tsee TSEE object to create the parallax for.
 * @param texture Texture to use for the object.
 * @param distanceFromCamera Distance from the camera to the object.
 * @return true on success, false on fail.
 */
bool TSEE_Parallax_Create(TSEE *tsee, TSEE_Texture *texture, float distanceFromCamera) {
	if (distanceFromCamera <= 0) {
		TSEE_Error("Distance from camera must be greater than 0 (Recieved %f)\n", distanceFromCamera);
		return false;
	}

	TSEE_Object *parallax = TSEE_Object_Create(tsee, texture, TSEE_ATTRIB_PARALLAX, 0, 0);
	if (!parallax) {
		return false;
	}

	parallax->texture = texture;
	parallax->parallax.distance = distanceFromCamera;
	parallax->texture->rect.y = tsee->window->height - texture->rect.h;
	bool inserted = false;

	for (size_t j = 0; j < tsee->world->objects->size; j++) {
		TSEE_Object *obj = TSEE_Array_Get(tsee->world->objects, j);
		if (!TSEE_Object_CheckAttribute(obj, TSEE_ATTRIB_PARALLAX)) {
			continue;
		}
		if (parallax->parallax.distance > obj->parallax.distance) {
			TSEE_Array_Insert(tsee->world->objects, parallax, j);
			inserted = true;
			break;
		}
	}

	if (!inserted) {
		TSEE_Array_Append(tsee->world->objects, parallax);
	}
	return true;
}

/**
 * @brief Create a parallax object from another object.
 * 
 * @param tsee TSEE object to create the parallax for.
 * @param obj Object to create the parallax object from.
 * @param distanceFromCamera Distance from the camera to the object.
 * @return true on success, false on fail.
 */
bool TSEE_Parallax_CreateFromObject(TSEE *tsee, TSEE_Object *obj, float distanceFromCamera) {
	return TSEE_Parallax_Create(tsee, obj->texture, distanceFromCamera);
}

/**
 * @brief Renders a parallax object in a TSEE object.
 * 
 * @param tsee TSEE object to render.
 * @param parallax Parallax object to render.
 * @return true on success, false on fail.
 */
bool TSEE_Parallax_Render(TSEE *tsee, TSEE_Object *parallax) {
	SDL_Rect newRect = parallax->texture->rect;
	newRect.x = tsee->world->scroll_x * (-1 / parallax->parallax.distance);
	while (newRect.x > tsee->window->width) {
		newRect.x -= tsee->window->width;
	}
	while (newRect.x + newRect.w < 0) {
		newRect.x += tsee->window->width;
	}
	if (SDL_RenderCopy(tsee->window->renderer, parallax->texture->texture, NULL, &newRect) != 0) {
		return false;
	};
	while (newRect.x + newRect.w < tsee->window->width) {
		newRect.x += tsee->window->width;
		if (SDL_RenderCopy(tsee->window->renderer, parallax->texture->texture, NULL, &newRect) != 0) {
			return false;
		};
	}
	// Render parallax again to left or right depending on scroll
	while (newRect.x > 0) {
		newRect.x -= newRect.w;
		if (SDL_RenderCopy(tsee->window->renderer, parallax->texture->texture, NULL, &newRect) != 0) {
			return false;
		};
	}
	while (newRect.x < 0) {
		newRect.x += newRect.w;
		if (SDL_RenderCopy(tsee->window->renderer, parallax->texture->texture, NULL, &newRect) != 0) {
			return false;
		};
	}
	return true;
}

/**
 * @brief Destroys a parallax object.
 * 
 * @param para Parallax object to destroy.
 * @param destroyTexture Whether to destroy the texture or not.
 */
void TSEE_Parallax_Destroy(TSEE_Object *para, bool destroyTexture) {
	if (destroyTexture) {
		TSEE_Texture_Destroy(para->texture);
	}
	xfree(para);
}