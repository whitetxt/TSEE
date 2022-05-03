#include "../tsee.h"

/**
 * @brief Create a TSEE_Array, to store anything.
 * 
 * @return TSEE_Array* 
 */
TSEE_Array *TSEE_Array_Create() {
	TSEE_Array *array = xmalloc(sizeof(*array));
	array->data = NULL;
	array->size = 0;
	return array;
} 

/**
 * @brief Increase an array's size by "size".
 * 
 * @param arr Array to extend
 * @param size Size to increase the array by.
 * @return int - The new size of the array.
 */
int TSEE_Array_Extend(TSEE_Array *arr, int size) {
	arr->data = xrealloc(arr->data, sizeof(*arr->data) * (arr->size + size));
	arr->size += size;
	return arr->size;
}

/**
 * @brief Append data to the end of an array.
 * 
 * @param arr Array to append to
 * @param data Data to append
 * @return true on success, false on fail.
 */
bool TSEE_Array_Append(TSEE_Array *arr, void *data) {
	TSEE_Array_Extend(arr, 1);
	arr->data[arr->size - 1] = data;
	return true;
}

/**
 * @brief Inserts an item into an array at the given index.
 * 
 * @param arr The array to insert into
 * @param data The data to insert into the array
 * @param index The index to insert the data at
 * @return true on success, false on fail.
 */
bool TSEE_Array_Insert(TSEE_Array *arr, void *data, size_t index) {
	if (!arr || !data) {
		TSEE_Error("Array or data is NULL\n");
		return false;
	}
	if (index >= arr->size) {
		TSEE_Warn("Attempted insert into array (size %zu) at index `%zu`\n", arr->size, index);
		return false;
	}
	TSEE_Array_Extend(arr, 1);
	for (size_t i = arr->size - 1; i > index; i--) {
		arr->data[i] = arr->data[i - 1];
	}
	// TODO: figure out why memmove was segfaulting
	//xmemmove(arr->data[index], arr->data[index + 1], sizeof(*arr->data) * (arr->size - 1 - index));
	arr->data[index] = data;
	return true;
}

/**
 * @brief Deletes an item at the given index from an array.
 * 
 * @param arr Array to delete from
 * @param index Index of the item to delete.
 * @return true on success, false on fail.
 */
bool TSEE_Array_Delete(TSEE_Array *arr, size_t index) {
	if (index >= arr->size) {
		TSEE_Warn("Attempted delete from array (size %zu) at index `%zu`\n", arr->size, index);
		return false;
	}
	for (size_t i = index; i < arr->size - 1; i++) {
		arr->data[i] = arr->data[i + 1];
	}
	// TODO: figure out why memmove was segfaulting
	//xmemmove(arr->data[index + 1], arr->data[index], sizeof(*arr->data) * (arr->size - index));
	arr->data = xrealloc(arr->data, sizeof(*arr->data) * (--arr->size));
	if (arr->size == 0 && arr->data) {
		xfree(arr->data);
		arr->data = NULL;
	}
	return true;
}

/**
 * @brief Gets the item at the given index from an array.
 * 
 * @param arr Array to get the item from
 * @param index Index of the item
 * @return void* 
 */
void *TSEE_Array_Get(TSEE_Array *arr, size_t index) {
	if (index >= arr->size) {
		TSEE_Warn("Attempted get from array (size %zu) at index `%zu`\n", arr->size, index);
		return NULL;
	}
	if (!arr->data[index]) TSEE_Warn("Data value in array at index `%zu` is NULL\n", index);
	return arr->data[index];
}

/**
 * @brief Clears an array of all its data.
 * Note: Does not free the data.
 * 
 * @param arr Array to clear
 * @return true on success, false on fail.
 */
bool TSEE_Array_Clear(TSEE_Array *arr) {
	if (!arr) return false;
	if (arr->data)
		xfree(arr->data);
	arr->data = NULL;
	arr->size = 0;
	return true;
}

/**
 * @brief Destroys an array, freeing itself.
 * Note: Does not free the array's data.
 * 
 * @param arr Array to destroy
 * @return true on success, false on fail.
 */
bool TSEE_Array_Destroy(TSEE_Array *arr) {
	if (!arr) return false;
	if (arr->data)
		xfree(arr->data);
	xfree(arr);
	return true;
}