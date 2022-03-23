#include "../tsee.h"

/* REWORKING OBJECTS */

/*
Objects:
	Type: TSEE_ENUM_OBJTYPE
	Texture: TSEE_Texture
	Position: TSEE_Vector
	Attributes: TSEE_Array<TSEE_Attribute>
*/

bool TSEEVec2Add(TSEE_Vec2 *final, TSEE_Vec2 *add) {
	final->x += add->x;
	final->y += add->y;
	return true;
}

bool TSEEVec2Normalise(TSEE_Vec2 *vec) {
	float len = sqrt(vec->x * vec->x + vec->y * vec->y);
	if (len == 0) {
		return false;
	}
	vec->x /= len;
	vec->y /= len;
	return true;
}

bool TSEEVec2Multiply(TSEE_Vec2 *vec, float mult) {
	vec->x *= mult;
	vec->y *= mult;
	return true;
}