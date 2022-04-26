#include "../tsee.h"

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

/**
 * @brief Divides a vector by a scalar.
 * 
 * @param vec Vector to divide
 * @param div Divisor
 * @return true on success, false on fail.
 */
bool TSEE_Vec2_Divide(TSEE_Vec2 *vec, float div) {
	vec->x /= div;
	vec->y /= div;
	return true;
}