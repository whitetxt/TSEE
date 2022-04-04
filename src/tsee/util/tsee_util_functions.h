// Creates a TSEE_Array with size 0.
TSEE_Array *TSEEArrayCreate();

// Extends a TSEE_Array by the specified size.
int TSEEArrayExtend(TSEE_Array *arr, int size);

// Appends data to the end of a TSEE_Array.
bool TSEEArrayAppend(TSEE_Array *arr, void *data);

// Insert data into TSEE_Array at the specified index.
bool TSEEArrayInsert(TSEE_Array *arr, void *toInsert, size_t index);

// Deletes data from a TSEE_Array at the specified index.
// This will move all following data to the left.
// Update any stored indexes after using this!
bool TSEEArrayDelete(TSEE_Array *arr, size_t index);

// Gets the specified index from a TSEE_Array.
void *TSEEArrayGet(TSEE_Array *arr, size_t index);

// Clears a TSEE_Array, deleting all data and setting size to 0.
bool TSEEArrayClear(TSEE_Array *arr);

// Frees a TSEE_Array, does not free data.
bool TSEEDestroyArray(TSEE_Array *arr);

// SAFETY

// Safely frees memory, and raises an error if it fails.
// USE xfree() NOT _xfree()
void _xfree(void *ptr, char *file, int line);

// Safely mallocs memory, and raises an error if it fails.
// USE xmalloc() NOT _xmalloc()
void *_xmalloc(size_t size, char *file, int line);

// Safely reallocs memory, and raises an error if it fails.
// USE xrealloc() NOT _xrealloc()
void *_xrealloc(void *ptr, size_t size, char *file, int line);

// Safely moves memory, and raises an error if it fails.
// USE xmemmove() NOT _xmemmove()
void *_xmemmove(void *dst, const void *src, size_t size, char *file, int line);

// Prints a backtrace to stdout.
void TSEEBacktrace();