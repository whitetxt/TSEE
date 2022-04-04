#include "../tsee.h"

TSEE_Array *TSEE_Array_Create() {
	TSEE_Array *array = xmalloc(sizeof(*array));
	array->data = NULL;
	array->size = 0;
	return array;
} 

int TSEE_Array_Extend(TSEE_Array *arr, int size) {
	arr->data = xrealloc(arr->data, sizeof(*arr->data) * (arr->size + size));
	arr->size += size;
	return arr->size - 1;
}

bool TSEE_Array_Append(TSEE_Array *arr, void *data) {
	int idx = TSEE_Array_Extend(arr, 1);
	arr->data[idx] = data;
	return true;
}

bool TSEE_Array_Insert(TSEE_Array *arr, void *data, size_t index) {
	if (index > arr->size) {
		TSEE_Warn("Attempted insert into array (size %zu) at index `%zu`\n", arr->size, index);
		return false;
	}
	TSEE_Array_Extend(arr, 1);
	xmemmove(arr->data[index + 1], arr->data[index + 2], sizeof(*arr->data) * (arr->size - index - 1));
	arr->data[index] = data;
	return true;
}

bool TSEE_Array_Delete(TSEE_Array *arr, size_t index) {
	if (index >= arr->size) {
		TSEE_Warn("Attempted delete from array (size %zu) at index `%zu`\n", arr->size, index);
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

void *TSEE_Array_Get(TSEE_Array *arr, size_t index) {
	if (index >= arr->size) {
		TSEE_Warn("Attempted get from array (size %zu) at index `%zu`\n", arr->size, index);
		return NULL;
	}
	return arr->data[index];
}

bool TSEE_Array_Clear(TSEE_Array *arr) {
	if (!arr) return false;
	if (arr->data)
		xfree(arr->data);
	arr->data = NULL;
	arr->size = 0;
	return true;
}

bool TSEE_Array_Destroy(TSEE_Array *arr) {
	if (!arr) return false;
	if (arr->data)
		xfree(arr->data);
	xfree(arr);
	return true;
}