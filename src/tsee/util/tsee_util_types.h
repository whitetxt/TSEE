// TSEE's 2D Vector type.
typedef struct TSEE_Vec2 {
	double x;
	double y;
} TSEE_Vec2;

typedef struct TSEE_Mat2 {
	double x0, x1;
	double y0, y1;
} TSEE_Mat2;

#define VEC_ZERO (TSEE_Vec2){0, 0};
#define RECT_NULL (SDL_Rect){0, 0, -1, -1};
#define MAT_ZERO (TSEE_Mat2){0, 0, 0, 0};

// TSEE's home-made array type. Stores the current size & data.
// Also has helper functions to insert, delete, append and more.
typedef struct TSEE_Array {
	void **data;
	size_t size;
} TSEE_Array;