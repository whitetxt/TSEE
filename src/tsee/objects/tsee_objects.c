#include "../tsee.h"

/**
 * @brief Check if an attribute is set on an object.
 * 
 * @param obj Object to check.
 * @param attr Attribute to check against.
 * @return true if it is set.
 */
bool TSEE_Object_CheckAttribute(TSEE_Object *obj, TSEE_Object_Attributes attr) {
	return TSEE_Attributes_Check(obj->attributes, attr);
}

/**
 * @brief Creates an object from a texture, with the given attributes.
 * 
 * @param tsee TSEE object
 * @param texture Texture to use for the object
 * @param attributes TSEE_Object_Attributes OR'd together
 * @param x X position for the object
 * @param y Y position for the object
 * @return TSEE_Object* or NULL
 */
TSEE_Object *TSEE_Object_Create(TSEE *tsee, TSEE_Texture *texture, TSEE_Object_Attributes attributes, float x, float y) {
	if (TSEE_Attributes_Check(attributes, TSEE_ATTRIB_UI) && TSEE_Attributes_Check(attributes, TSEE_ATTRIB_PHYS_ENABLED)) {
		TSEE_Error("Cannot create object with UI and physics attributes.\n");
		return NULL;
	}
	TSEE_Object *obj = xmalloc(sizeof(*obj));
	obj->texture = texture;
	TSEE_Object_SetPosition(obj, x, y);

	if (TSEE_Attributes_Check(attributes, TSEE_ATTRIB_PLAYER)) {
		if (TSEE_Attributes_Check(attributes, TSEE_ATTRIB_UI)) {
			TSEE_Error("Cannot create object with player and UI attributes.\n");
			xfree(obj);
			return NULL;
		}
		TSEE_Attributes_Set(&attributes, TSEE_ATTRIB_PHYS_ENABLED);
		tsee->player->object = obj;
		tsee->player->movement = (TSEE_Player_Movement){0, 0, 0, 0};
	}

	if (TSEE_Attributes_Check(attributes, TSEE_ATTRIB_PHYS_ENABLED)) {
		obj->physics.mass = 1;
		obj->physics.inv_mass = 1;
		obj->physics.velocity = (TSEE_Vec2){0, 0};
		obj->physics.force = (TSEE_Vec2){0, 0};
	}

	obj->attributes = attributes;

	if (TSEE_Attributes_Check(attributes, TSEE_ATTRIB_PARALLAX)) TSEE_Array_Insert(tsee->world->objects, obj, 0);
	else TSEE_Array_Append(tsee->world->objects, obj);

	return obj;
}

/**
 * @brief Set the position of an object
 * 
 * @param obj Object to set
 * @param x New X position
 * @param y New Y position
 * @return true on success, false on failure
 */
bool TSEE_Object_SetPosition(TSEE_Object *obj, float x, float y) {
	if (!obj) {
		TSEE_Error("Attempted to set position on NULL pointer.\n");
		return false;
	}
	obj->position.x = x;
	obj->position.y = y;
	if (!obj->texture) {
		TSEE_Error("Cannot set position of object with no texture.\n");
		return false;
	}
	obj->texture->rect.x = x;
	obj->texture->rect.y = y;
	return true;
}

/**
 * @brief Gets the collision rectangle for two objects colliding
 * 
 * @param obj First object
 * @param other Second object
 * @return SDL_Rect - Check with TSEE_IsNullRect to verify if it is real.
 */
SDL_Rect TSEE_Object_GetCollisionRect(TSEE *tsee, TSEE_Object *obj, TSEE_Object *other) {
	SDL_Rect new_rect;
	SDL_Rect obj_rect = TSEE_Object_GetRect(tsee, obj);
	SDL_Rect other_rect = TSEE_Object_GetRect(tsee, other);
	if (SDL_IntersectRect(&obj_rect, &other_rect, &new_rect) == SDL_TRUE) {
		return new_rect;
	}
	return (SDL_Rect){0, 0, -1, -1};
}

SDL_Rect TSEE_Object_GetRect(TSEE *tsee, TSEE_Object *obj) {
	return (SDL_Rect){obj->position.x, obj->position.y * -1 + tsee->window->height, obj->texture->rect.w, obj->texture->rect.h};
}

/**
 * @brief Renders an object
 * 
 * @param tsee TSEE to render to
 * @param object Object to render
 * @return true on success, false on fail
 */
bool TSEE_Object_Render(TSEE *tsee, TSEE_Object *object) {
	if (!TSEE_Object_CheckAttribute(object, TSEE_ATTRIB_PARALLAX)) {
		SDL_Rect rect = TSEE_Object_GetRect(tsee, object);
		int ret = SDL_RenderCopy(tsee->window->renderer, object->texture->texture, NULL, &rect);
		if (ret != 0) {
			TSEE_Error("Failed to render object: %s\n", SDL_GetError());
			return false;
		}
	} else {
		if (!TSEE_Parallax_Render(tsee, object)) {
			TSEE_Error("Failed to render parallax: %s\n", SDL_GetError());
			return false;
		}
	}
	return true;
}

/**
 * @brief Destroys an object.
 * 
 * @param object Object to destroy
 * @param destroyTexture Whether to destroy the texture or not
 */
void TSEE_Object_Destroy(TSEE *tsee, TSEE_Object *object, bool destroyTexture) {
	if (destroyTexture) {
		TSEE_Texture_Destroy(tsee, object->texture);
	}
	if (TSEE_Object_CheckAttribute(object, TSEE_ATTRIB_TEXT)) {
		xfree(object->text.text);
	}
	xfree(object);
}