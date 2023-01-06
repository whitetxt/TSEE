#include "../tsee.h"

/**
 * @brief Adds two vectors, and stores the result in the first vector.
 *
 * @param final Destination vector
 * @param add Vector to add
 * @return success status
 */
bool TSEE_Vec2_Add(TSEE_Vec2 *final, TSEE_Vec2 add) {
	if (!final) {
		TSEE_Error("Attempted to add NULL vectors.\n");
		return false;
	}
	final->x += add.x;
	final->y += add.y;
	return true;
}

/**
 * @brief Adds two vectors, and returns the resulting vector
 *
 * @param final Destination vector
 * @param add Vector to add
 * @return the resulting vector
 */
TSEE_Vec2 TSEE_Vec2_RAdd(TSEE_Vec2 final, TSEE_Vec2 add) {
	TSEE_Vec2 res = final;
	res.x += add.x;
	res.y += add.y;
	return res;
}

/**
 * @brief Returns the length of the vector
 * 
 * @param vec Vector to get the length of.
 * @return length of the vector.
 */
double TSEE_Vec2_Len(TSEE_Vec2 vec) {
	return sqrt(vec.x * vec.x + vec.y * vec.y);
}

/**
 * @brief Returns the length of the vector squared.
 * 
 * @param vec Vector to get the length of.
 * @return length of the vector squared.
 */
double TSEE_Vec2_SqLen(TSEE_Vec2 vec) {
	return vec.x * vec.x + vec.y * vec.y;
}

/**
 * @brief Normalises a vector, and stores the result in the vector.
 *
 * @param vec Vector to normalise
 * @return success status
 */
bool TSEE_Vec2_Normalise(TSEE_Vec2 *vec) {
	if (!vec) {
		TSEE_Error("Attempted to normalise a NULL vector\n");
		return false;
	}
	if (vec->x + vec->y == 0) {
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
 * @brief Normalises a vector, and returns the resulting vector.
 *
 * @param vec Vector to normalise
 * @return the resulting vector
 */
TSEE_Vec2 TSEE_Vec2_RNormalise(TSEE_Vec2 vec) {
	double len = sqrt(vec.x * vec.x + vec.y * vec.y);
	if (len == 0) {
		return VEC_ZERO;
	}
	TSEE_Vec2 res = vec;
	res.x /= len;
	res.y /= len;
	return res;
}

/**
 * @brief Multiplies a vector by a scalar, and stores the result in the vector.
 *
 * @param vec Vector to multiply.
 * @param mult Multiplier
 * @return success status
 */
bool TSEE_Vec2_Multiply(TSEE_Vec2 *vec, double mult) {
	if (!vec) {
		TSEE_Error("Attempted to multiply a NULL vector\n");
		return false;
	}
	vec->x *= mult;
	vec->y *= mult;
	return true;
}

/**
 * @brief Multiplies a vector by a scalar, and returns the resulting vector.
 *
 * @param vec Vector to multiply.
 * @param mult Multiplier
 * @return the resulting vector.
 */
TSEE_Vec2 TSEE_Vec2_RMultiply(TSEE_Vec2 vec, double mult) {
	TSEE_Vec2 res = vec;
	res.x *= mult;
	res.y *= mult;
	return res;
}

/**
 * @brief Divides a vector by a scalar, and stores the result in the vector.
 *
 * @param vec Vector to divide
 * @param div Divisor
 * @return success status
 */
bool TSEE_Vec2_Divide(TSEE_Vec2 *vec, double div) {
	if (!vec) {
		TSEE_Error("Attempted to divide a NULL vector\n");
		return false;
	}
	vec->x /= div;
	vec->y /= div;
	return true;
}

/**
 * @brief Divides a vector by a scalar and returns the resulting vector
 *
 * @param vec Vector to divide
 * @param div Divisor
 * @return TSEE_Vec2 resulting vector
 */
TSEE_Vec2 TSEE_Vec2_RDivide(TSEE_Vec2 vec, double div) {
	TSEE_Vec2 res = vec;
	res.x /= div;
	res.y /= div;
	return res;
}

/**
 * @brief Calculate the dot product of two vectors.
 *
 * @param first First vector
 * @param second Second vector
 * @return double The resultant dot product
 */
double TSEE_Vec2_Dot(TSEE_Vec2 first, TSEE_Vec2 second) {
	return first.x * second.x + first.y * second.y;
}

/**
 * @brief Subtract one vector from another, storing the result in the first
 * vector.
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

/**
 * @brief Subtract one vector from another, returning the resultant vector
 * 
 * @param first the first vector
 * @param minus the vector to subtract
 * @return TSEE_Vec2 the resultant vector
 */
TSEE_Vec2 TSEE_Vec2_RSubtract(TSEE_Vec2 first, TSEE_Vec2 minus) {
	TSEE_Vec2 ret = first;
	ret.x -= minus.x;
	ret.y -= minus.y;
	return ret;
}