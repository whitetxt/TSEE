#include "include/tsee.h"

int TSEEArrayExtend(TSEE_Array *arr) {
	arr->data = realloc(arr->data, sizeof(*arr->data) * (++arr->size));
	return arr->size - 1;
}

bool TSEEArrayAppend(TSEE_Array *arr, void *data) {
	int idx = TSEEArrayExtend(arr);
	arr->data[idx] = data;
	return true;
}

bool TSEEArrayInsert(TSEE_Array *arr, void *data, int index) {
	if (index > arr->size) {
		TSEEWarn("Attempted insert into array (size %d) at index \"%d\"\n", arr->size, index);
		return false;
	}
	TSEEArrayExtend(arr);
	memmove(arr->data[index + 1], arr->data[index + 2], sizeof(*arr->data) * (arr->size - index));
	arr->data[index] = data;
	return true;
}

bool TSEEArrayDelete(TSEE_Array *arr, int index) {
	if (index >= arr->size) {
		TSEEWarn("Attempted delete from array (size %d) at index \"%d\"\n", arr->size, index);
		return false;
	}
	memmove(arr->data[index + 1], arr->data[index], sizeof(*arr->data) * (arr->size - index));
	arr->data = realloc(arr->data, sizeof(*arr->data) * (--arr->size));
	return true;
}

void *TSEEArrayGet(TSEE_Array *arr, int index) {
	if (index >= arr->size) {
		TSEEWarn("Attempted get from array (size %d) at index \"%d\"\n", arr->size, index);
		return NULL;
	}
	return arr->data[index];
}

bool TSEEArrayClear(TSEE_Array *arr) {
	free(arr);
	return true;
}