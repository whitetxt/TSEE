#include "../tsee.h"

/**
 * @brief Check if an attribute is set on an object.
 * 
 * @param obj Object to check.
 * @param attr Attribute to check against.
 * @return true if it is set.
 */
bool TSEE_Object_CheckAttribute(TSEE_Object *obj, TSEE_Object_Attributes attr) {
	return obj->attributes & attr;
}

/**
 * @brief Check if an attribute is set in an attribute.
 * 
 * @param attr Attribute to check.
 * @param attr2 Attribute to check against.
 * @return true if it is set.
 */
bool TSEE_Attributes_Check(TSEE_Object_Attributes attr, TSEE_Object_Attributes attr2) {
	return attr & attr2;
}

/**
 * @brief Creates an object from 
 * 
 * @param tsee TSEE object
 * @param texture Texture to use for the object
 * @param attributes TSEE_Object_Attributes OR'd together
 * @param x X position for the object
 * @param y Y position for the object
 * @return TSEE_Object* or NULL
 */
TSEE_Object *TSEE_Object_Create(TSEE *tsee, TSEE_Texture *texture, TSEE_Object_Attributes attributes, float x, float y) {
	if (TSEE_Attributes_Check(attributes, TSEE_STATIC) && TSEE_Attributes_Check(attributes, TSEE_PHYSICS_ENABLED)) {
		TSEE_Error("Cannot create object with static and physics attributes.\n");
		return NULL;
	}
	TSEE_Object *obj = xmalloc(sizeof(*obj));
	obj->texture = texture;
	obj->position.x = x;
	obj->position.y = y;
	obj->attributes = attributes;
	TSEE_Array_Append(tsee->world->objects, &obj);
	return obj;
}

/**
 * @brief Adds two vectors
 * 
 * @param final Destination vector
 * @param add Vector to add
 * @return true on success
 */
bool TSEE_Vec2_Add(TSEE_Vec2 *final, TSEE_Vec2 *add) {
	final->x += add->x;
	final->y += add->y;
	return true;
}

/**
 * @brief Normalises a vector.
 * 
 * @param vec Vector to normalise.
 * @return true on success, false on fail.
 */
bool TSEE_Vec2_Normalise(TSEE_Vec2 *vec) {
	float len = sqrt(vec->x * vec->x + vec->y * vec->y);
	if (len == 0) {
		return false;
	}
	vec->x /= len;
	vec->y /= len;
	return true;
}

/**
 * @brief Multiplies a vector by a scalar.
 * 
 * @param vec Vector to multiply.
 * @param mult Multiplier
 * @return true on success, false on fail.
 */
bool TSEE_Vec2_Multiply(TSEE_Vec2 *vec, float mult) {
	vec->x *= mult;
	vec->y *= mult;
	return true;
}