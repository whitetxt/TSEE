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

void *xmalloc(size_t size) {
	void *ptr = malloc(size);
	if (!ptr) {
		TSEEError("Failed to allocate %zu bytes\n", size);
		TSEEBacktrace();
		return NULL;
	}
	return ptr;
}

void xfree(void *ptr) {
	if (ptr) {
		free(ptr);
	} else {
		TSEEError("Tried to free NULL pointer.\n");
		TSEEBacktrace();
	}
}