// TSEE's 2D Vector type.
typedef struct TSEE_Vec2 {
	double x;
	double y;
} TSEE_Vec2;

#define VEC_ZERO (TSEE_Vec2){0, 0};
#define RECT_NULL (SDL_Rect){0, 0, -1, -1};

// TSEE's home-made array type. Stores the current size & data.
// Also has helper functions to insert, delete, append and more.
typedef struct TSEE_Array {
	void **data;
	size_t size;
} TSEE_Array;