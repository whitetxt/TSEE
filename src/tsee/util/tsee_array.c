#include "../tsee.h"

TSEE_Array *TSEEArrayCreate() {
	TSEE_Array *array = xmalloc(sizeof(*array));
	array->data = NULL;
	array->size = 0;
	return array;
} 

int TSEEArrayExtend(TSEE_Array *arr, int size) {
	arr->data = xrealloc(arr->data, sizeof(*arr->data) * (arr->size + size));
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
		TSEEWarn("Attempted insert into array (size %zu) at index `%zu`\n", arr->size, index);
		return false;
	}
	TSEEArrayExtend(arr, 1);
	xmemmove(arr->data[index + 1], arr->data[index + 2], sizeof(*arr->data) * (arr->size - index - 1));
	arr->data[index] = data;
	return true;
}

bool TSEEArrayDelete(TSEE_Array *arr, size_t index) {
	if (index >= arr->size) {
		TSEEWarn("Attempted delete from array (size %zu) at index `%zu`\n", arr->size, index);
		return false;
	}
	xmemmove(arr->data[index + 1], arr->data[index], sizeof(*arr->data) * (arr->size - index));
	arr->data = xrealloc(arr->data, sizeof(*arr->data) * (--arr->size));
	if (arr->size == 0) {
		xfree(arr->data);
		arr->data = NULL;
	}
	return true;
}

void *TSEEArrayGet(TSEE_Array *arr, size_t index) {
	if (index >= arr->size) {
		TSEEWarn("Attempted get from array (size %zu) at index `%zu`\n", arr->size, index);
		return NULL;
	}
	return arr->data[index];
}

bool TSEEArrayClear(TSEE_Array *arr) {
	if (!arr) {
		return false;
	}
	if (arr->data)
		xfree(arr->data);
	arr->data = NULL;
	arr->size = 0;
	return true;
}

bool TSEEDestroyArray(TSEE_Array *arr) {
	if (!arr) {
		return false;
	}
	if (arr->data) {
		xfree(arr->data);
	}
	xfree(arr);
	return true;
}