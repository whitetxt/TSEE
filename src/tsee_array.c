#include "include/tsee.h"

TSEE_Array *TSEEArrayCreate() {
	TSEE_Array *array = malloc(sizeof(*array));
	array->data = NULL;
	array->size = 0;
	return array;
} 

int TSEEArrayExtend(TSEE_Array *arr, int size) {
	arr->data = realloc(arr->data, sizeof(*arr->data) * (arr->size + size));
	arr->size += size;
	return arr->size - 1;
}

bool TSEEArrayAppend(TSEE_Array *arr, void *data) {
	int idx = TSEEArrayExtend(arr, 1);
	arr->data[idx] = data;
	return true;
}

bool TSEEArrayInsert(TSEE_Array *arr, void *data, size_t index) {
	if (index > arr->size) {
		TSEEWarn("Attempted insert into array (size %d) at index `%d`\n", arr->size, index);
		return false;
	}
	TSEEArrayExtend(arr, 1);
	memmove(arr->data[index + 1], arr->data[index + 2], sizeof(*arr->data) * (arr->size - index - 1));
	arr->data[index] = data;
	return true;
}

bool TSEEArrayDelete(TSEE_Array *arr, size_t index) {
	if (index >= arr->size) {
		TSEEWarn("Attempted delete from array (size %d) at index `%d`\n", arr->size, index);
		return false;
	}
	memmove(arr->data[index + 1], arr->data[index], sizeof(*arr->data) * (arr->size - index));
	arr->data = realloc(arr->data, sizeof(*arr->data) * (--arr->size));
	return true;
}

void *TSEEArrayGet(TSEE_Array *arr, size_t index) {
	if (index >= arr->size) {
		TSEEWarn("Attempted get from array (size %d) at index `%d`\n", arr->size, index);
		return NULL;
	}
	return arr->data[index];
}

bool TSEEArrayClear(TSEE_Array *arr) {
	if (!arr) {
		return false;
	}
	for (size_t i = 0; i < arr->size; i++) {
		free(arr->data[i]);
	}
	free(arr->data);
	arr->data = NULL;
	arr->size = 0;
	return true;
}

bool TSEEArrayFree(TSEE_Array *arr) {
	if (!arr) {
		return false;
	}
	for (size_t i = 0; i < arr->size; i++) {
		free(arr->data[i]);
	}
	free(arr->data);
	free(arr);
	return true;
}