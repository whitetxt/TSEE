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
		TSEE_Error("Cannot create object with static and physics attributes.\n");
		return NULL;
	}
	TSEE_Object *obj = xmalloc(sizeof(*obj));
	obj->texture = texture;
	obj->position.x = x;
	obj->position.y = y;
	if (TSEE_Attributes_Check(attributes, TSEE_ATTRIB_PLAYER)) {
		TSEE_Attributes_Set(attributes, TSEE_ATTRIB_PHYS_ENABLED);
		tsee->player->object = obj;
		tsee->player->movement = (TSEE_Player_Movement){0, 0, 0, 0};
	}
	obj->attributes = attributes;
	TSEE_Array_Append(tsee->world->objects, obj);
	return obj;
}

/**
 * @brief Renders an object
 * 
 * @param tsee TSEE to render to
 * @param object Object to render
 * @return true on success, false on fail
 */
bool TSEE_Object_Render(TSEE *tsee, TSEE_Object *object) {
	int ret = SDL_RenderCopy(tsee->window->renderer, object->texture->texture, NULL, &object->texture->rect);
	if (ret != 0) {
		TSEE_Error("Failed to render object: %s\n", SDL_GetError());
		return false;
	}
	return true;
}