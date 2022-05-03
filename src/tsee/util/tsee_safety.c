#include "../tsee.h"
#include <execinfo.h>

/**
 * @brief Prints a backtrace.
 * 
 */
void TSEE_Backtrace() {
	void *arr[10];
	char **strings;
	int size, i;

	size = backtrace(arr, 10);
	strings = backtrace_symbols(arr, size);
	if (strings) {
		printf("Backtrace:\n");
		for (i = 0; i < size; i++)
			printf("%s\n", strings[i]);
	}
	xfree(strings);
}

/**
 * @brief Safe wrapper for malloc
 * 
 * @param size Size to allocate
 * @param file File which called this function
 * @param line Line which called this function
 * @return void* 
 */
void *_xmalloc(size_t size, char *file, int line) {
	void *ptr = malloc(size);
	if (!ptr) {
		TSEE_Error("Failed to allocate %zu bytes\nCalled from %s:%d\n", size, file, line);
		TSEE_Backtrace();
		return NULL;
	}
	return ptr;
}

/**
 * @brief Safe wrapper for free
 * 
 * @param ptr Pointer to free
 * @param file File which called this function
 * @param line Line which called this function
 */
void _xfree(void *ptr, char *file, int line) {
	if (ptr) {
		free(ptr);
	} else {
		TSEE_Error("Tried to free NULL pointer.\nCalled from %s:%d\n", file, line);
		TSEE_Backtrace();
	}
}

/**
 * @brief Safe wrapper for realloc.
 * 
 * @param ptr Pointer to realloc
 * @param size Size of the new allocation
 * @param file File which called this function
 * @param line Line which called this function
 * @return void* 
 */
void *_xrealloc(void *ptr, size_t size, char *file, int line) {
	void *newPtr = realloc(ptr, size);
	if (!newPtr && size != 0) {
		TSEE_Error("Failed to realloc %zu bytes\nCalled from %s:%d\n", size, file, line);
		TSEE_Backtrace();
		return NULL;
	}
	return newPtr;
}

/**
 * @brief Safe wrapper for memmove
 * 
 * @param src Source pointer
 * @param dst Destination pointer
 * @param size Size of memory to move
 * @param file File which called this function
 * @param line Line which called this function
 * @return void* 
 */
void *_xmemmove(const void *src, void *dst, size_t size, char *file, int line) {
	void *newPtr = memmove(dst, src, size);
	if (!newPtr) {
		TSEE_Error("Failed to memmove %zu bytes\nCalled from %s:%d\n", size, file, line);
		TSEE_Backtrace();
		return NULL;
	}
	return newPtr;
}

/**
 * @brief Checks if an SDL_Rect is null
 * 
 * @param rect The rect to check
 * @return true if width + height are both < 0
 */
bool TSEE_IsRectNull(SDL_Rect rect) {
	return rect.w < 0 && rect.h < 0;
}