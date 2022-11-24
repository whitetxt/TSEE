// Util

TSEE_Array *TSEE_Array_Create();
int TSEE_Array_Extend(TSEE_Array *arr, int size);
bool TSEE_Array_Append(TSEE_Array *arr, void *data);
bool TSEE_Array_Insert(TSEE_Array *arr, void *toInsert, size_t index);
bool TSEE_Array_Delete(TSEE_Array *arr, size_t index);
bool TSEE_Array_DeleteItem(TSEE_Array *arr, void *data);
void *TSEE_Array_Get(TSEE_Array *arr, size_t index);
int TSEE_Array_GetIndex(TSEE_Array *arr, void *data);
bool TSEE_Array_Clear(TSEE_Array *arr);
bool TSEE_Array_Destroy(TSEE_Array *arr);

// Safety

void _xfree(void *ptr, char *file, int line);
void *_xmalloc(size_t size, char *file, int line);
void *_xrealloc(void *ptr, size_t size, char *file, int line);
void *_xmemmove(const void *src, void *dst, size_t size, char *file, int line);
void TSEE_Backtrace();
bool TSEE_IsRectNull(SDL_Rect rect);

// Vector

bool TSEE_Vec2_Add(TSEE_Vec2 *final, TSEE_Vec2 add);
TSEE_Vec2 TSEE_Vec2_RAdd(TSEE_Vec2 final, TSEE_Vec2 add);
double TSEE_Vec2_Len(TSEE_Vec2 vec);
double TSEE_Vec2_SqLen(TSEE_Vec2 vec);
bool TSEE_Vec2_Normalise(TSEE_Vec2 *vec);
TSEE_Vec2 TSEE_Vec2_RNormalise(TSEE_Vec2 vec);
bool TSEE_Vec2_Multiply(TSEE_Vec2 *vec, double mult);
TSEE_Vec2 TSEE_Vec2_RMultiply(TSEE_Vec2 vec, double mult);
bool TSEE_Vec2_Divide(TSEE_Vec2 *vec, double div);
TSEE_Vec2 TSEE_Vec2_RDivide(TSEE_Vec2 vec, double div);
double TSEE_Vec2_Dot(TSEE_Vec2 first, TSEE_Vec2 second);
bool TSEE_Vec2_Subtract(TSEE_Vec2 *first, TSEE_Vec2 minus);
TSEE_Vec2 TSEE_Vec2_RSubtract(TSEE_Vec2 first, TSEE_Vec2 minus);

#define min(a, b) a < b ? a : b;