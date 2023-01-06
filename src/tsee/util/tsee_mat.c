#include "../tsee.h"

/**
 * @brief Transposes a 2x2 matrix.
 * 
 * @param mat matrix to transpose
 * @return success status
 */
bool TSEE_Mat2_transpose(TSEE_Mat2 *mat) {
	double x1 = mat->x1;
	mat->x1 = mat->y0;
	mat->y0 = x1;
	return true;
}

TSEE_Vec2 TSEE_Mat2_VecMultiply(TSEE_Mat2 mat, TSEE_Vec2 vec) {
	return (TSEE_Vec2){mat.x0 * vec.x + mat.x1 * vec.y, mat.y0 * vec.x + mat.y1 * vec.y};
}