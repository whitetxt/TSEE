#include "include/tsee.h"
#include <execinfo.h>

void TSEEBacktrace() {
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

void *_xmalloc(size_t size, char *file, int line) {
	void *ptr = malloc(size);
	if (!ptr) {
		TSEEError("Failed to allocate %zu bytes\nCalled from %s:%d\n", size, file, line);
		TSEEBacktrace();
		return NULL;
	}
	return ptr;
}

void _xfree(void *ptr, char *file, int line) {
	if (ptr) {
		free(ptr);
	} else {
		TSEEError("Tried to free NULL pointer.\nCalled from %s:%d\n", file, line);
		TSEEBacktrace();
	}
}

void *_xrealloc(void *ptr, size_t size, char *file, int line) {
	void *newPtr = realloc(ptr, size);
	if (!newPtr) {
		TSEEError("Failed to realloc %zu bytes\nCalled from %s:%d\n", size, file, line);
		TSEEBacktrace();
		return NULL;
	}
	return newPtr;
}

void *_xmemmove(void *dst, const void *src, size_t size, char *file, int line) {
	void *newPtr = memmove(dst, src, size);
	if (!newPtr) {
		TSEEError("Failed to memmove %zu bytes\nCalled from %s:%d\n", size, file, line);
		TSEEBacktrace();
		return NULL;
	}
	return newPtr;
}
