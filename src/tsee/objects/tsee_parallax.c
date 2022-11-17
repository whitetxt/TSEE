#include "../tsee.h"

/**
 * @brief Creates a parallax object from a texture.
 * 
 * @param tsee TSEE object to create the parallax for.
 * @param texture Texture to use for the object.
 * @param distanceFromCamera Distance from the camera to the object.
 * @return The new object (or NULL on failure).
 */
TSEE_Object *TSEE_Parallax_Create(TSEE *tsee, TSEE_Texture *texture, float distanceFromCamera) {
	if (distanceFromCamera <= 0) {
		TSEE_Error("Distance from camera must be greater than 0 (Recieved %f)\n", distanceFromCamera);
		return NULL;
	}

	TSEE_Object *parallax = TSEE_Object_Create(tsee, texture, TSEE_ATTRIB_PARALLAX, 0, 0);
	if (!parallax) {
		return NULL;
	}

	parallax->parallax.distance = distanceFromCamera;
	parallax->texture->rect.y = tsee->window->height - texture->rect.h;

	size_t latest_good_index = 0;

	for (size_t i = 1; i < tsee->world->objects->size; i++) {
		TSEE_Object *obj = TSEE_Array_Get(tsee->world->objects, i);
		if (!TSEE_Object_CheckAttribute(obj, TSEE_ATTRIB_PARALLAX)) continue;

		if (parallax->parallax.distance < obj->parallax.distance) latest_good_index = i;
		else break;
	}

	TSEE_Array_Delete(tsee->world->objects, 0);
	TSEE_Array_Insert(tsee->world->objects, parallax, latest_good_index);
	return parallax;
}

/**
 * @brief Create a parallax object from another object.
 * 
 * @param tsee TSEE object to create the parallax for.
 * @param obj Object to create the parallax object from.
 * @param distanceFromCamera Distance from the camera to the object.
 * @return The new object.
 */
TSEE_Object *TSEE_Parallax_CreateFromObject(TSEE *tsee, TSEE_Object *obj, float distanceFromCamera) {
	return TSEE_Parallax_Create(tsee, obj->texture, distanceFromCamera);
}

/**
 * @brief Renders a parallax object in a TSEE object.
 * 
 * @param tsee TSEE object to render.
 * @param parallax Parallax object to render.
 * @return success status
 */
bool TSEE_Parallax_Render(TSEE *tsee, TSEE_Object *parallax) {
	Uint64 start = 0;
	if (tsee->debug->active) {
		start = SDL_GetPerformanceCounter();
	}
	if (!TSEE_Object_CheckAttribute(parallax, TSEE_ATTRIB_PARALLAX)) {
		TSEE_Error("Attempted to parallax render a non parallax object.\n");
		return false;
	}
	parallax->texture->rect.x = tsee->world->scroll_x * (-1 / parallax->parallax.distance);
	while (parallax->texture->rect.x > tsee->window->width) {
		parallax->texture->rect.x -= tsee->window->width;
	}
	while (parallax->texture->rect.x + parallax->texture->rect.w < 0) {
		parallax->texture->rect.x += tsee->window->width;
	}
	while (parallax->texture->rect.x > 0) {
		parallax->texture->rect.x -= parallax->texture->rect.w;
	}
	if (SDL_RenderCopy(tsee->window->renderer, parallax->texture->texture, NULL, &parallax->texture->rect) != 0) {
		return false;
	};
	while (parallax->texture->rect.x + parallax->texture->rect.w < tsee->window->width) {
		parallax->texture->rect.x += parallax->texture->rect.w;
		if (SDL_RenderCopy(tsee->window->renderer, parallax->texture->texture, NULL, &parallax->texture->rect) != 0) {
			return false;
		};
	}
	if (tsee->debug->active) {
		Uint64 end = SDL_GetPerformanceCounter();
		tsee->debug->render_times.parallax_time += (end - start) * 1000 / (double) SDL_GetPerformanceFrequency();
	}
	return true;
}

/**
 * @brief Destroys a parallax object.
 * 
 * @param para Parallax object to destroy.
 * @param destroyTexture Whether to destroy the texture or not.
 */
void TSEE_Parallax_Destroy(TSEE *tsee, TSEE_Object *para, bool destroyTexture) {
	if (destroyTexture) {
		TSEE_Texture_Destroy(para->texture);
		TSEE_Resource_Texture_Delete(tsee, para->texture);
	}
	xfree(para);
}