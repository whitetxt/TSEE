#include "../tsee.h"

/**
 * @brief Adds two vectors
 * 
 * @param final Destination vector
 * @param add Vector to add
 * @return true on success
 */
bool TSEE_Vec2_Add(TSEE_Vec2 *final, TSEE_Vec2 *add) {
	if (!final || !add) {
		TSEE_Error("Attempted to add NULL vectors.\n");
		return false;
	}
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
	if (!vec) {
		TSEE_Error("Attempted to normalise a NULL vector\n");
		return false;
	}

	double len = sqrt(vec->x * vec->x + vec->y * vec->y);
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
	if (!vec) {
		TSEE_Error("Attempted to multiply a NULL vector\n");
		return false;
	}
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
	if (!vec) {
		TSEE_Error("Attempted to divide a NULL vector\n");
		return false;
	}
	vec->x /= div;
	vec->y /= div;
	return true;
}

/**
 * @brief Copies a Vec2 into a new one.
 * 
 * @param copy Vec2 to copy
 * @return TSEE_Vec2 
 */
TSEE_Vec2 TSEE_Vec2_Copy(TSEE_Vec2 copy) {
	return (TSEE_Vec2){copy.x, copy.y};
}

/**
 * @brief Calculate the dot product of two vectors.
 * 
 * @param first First vector
 * @param second Second vector
 * @return float The resultant dot product
 */
float TSEE_Vec2_Dot(TSEE_Vec2 first, TSEE_Vec2 second) {
	return first.x * second.x + first.y * second.y;
}

/**
 * @brief Subtract one vector from another, storing the result in the first vector.
 * 
 * @param first The first vector
 * @param minus The vector to subtract
 * @return true on success
 */
bool TSEE_Vec2_Subtract(TSEE_Vec2 *first, TSEE_Vec2 minus) {
	if (!first) {
		TSEE_Error("Attempted to subtract a NULL vector\n");
		return false;
	}
	first->x -= minus.x;
	first->y -= minus.y;
	return true;
}