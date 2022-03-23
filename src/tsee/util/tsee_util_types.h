// TSEE's 2D Vector type.
typedef struct TSEE_Vec2 {
	float x;
	float y;
} TSEE_Vec2;

// TSEE's home-made array type. Stores the current size & data.
// Also has helper functions to insert, delete, append and more.
typedef struct TSEE_Array {
	void **data;
	size_t size;
} TSEE_Array;