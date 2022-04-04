#include "../tsee.h"

/* REWORKING OBJECTS */

/*
Objects:
	Texture: TSEE_Texture
	Position: TSEE_Vector
	Attributes: TSEE_Array<TSEE_Attribute>
*/

/**
 * @brief Creates an object from 
 * 
 * @param tsee TSEE Object
 * @param texture Texture to use
 * @param attributes TSEE_Object_Attributes OR'd together
 * @return TSEE_Object 
 */
TSEE_Object TSEE_Create_Object(TSEE *tsee, TSEE_Texture *texture, TSEE_Object_Attributes attributes) {
	TSEE_Object obj = {
		.texture = texture,
		.position = {0, 0},
		.attributes = attributes,
	};
	TSEE_Array_Append(tsee->world->objects, &obj);
	return obj;
}

bool TSEE_Vec2_Add(TSEE_Vec2 *final, TSEE_Vec2 *add) {
	final->x += add->x;
	final->y += add->y;
	return true;
}

bool TSEE_Vec2_Normalise(TSEE_Vec2 *vec) {
	float len = sqrt(vec->x * vec->x + vec->y * vec->y);
	if (len == 0) {
		return false;
	}
	vec->x /= len;
	vec->y /= len;
	return true;
}

bool TSEE_Vec2_Multiply(TSEE_Vec2 *vec, float mult) {
	vec->x *= mult;
	vec->y *= mult;
	return true;
}