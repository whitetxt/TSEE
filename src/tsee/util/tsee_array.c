#include "../tsee.h"

/**
 * @brief Create a TSEE_Array, to store anything.
 *
 * @return TSEE_Array*
 */
TSEE_Array *TSEE_Array_Create() {
	TSEE_Array *array = xmalloc(sizeof(*array));
	if (!array) {
		TSEE_Error("Failed to malloc for array.\n");
		return NULL;
	}
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
 * @return success status
 */
bool TSEE_Array_Append(TSEE_Array *arr, void *data) {
	TSEE_Array_Extend(arr, 1);
	arr->data[arr->size - 1] = data;
	return true;
}

/**
 * @brief Inserts an item into an array at the given index.
 *
 * @todo Find out why memmove was segfaulting, for extra performance.
 *
 * @param arr The array to insert into
 * @param data The data to insert into the array
 * @param index The index to insert the data at
 * @return success status
 */
bool TSEE_Array_Insert(TSEE_Array *arr, void *data, size_t index) {
	if (!arr || !data) {
		TSEE_Error("Array or data is NULL\n");
		return false;
	}
	if (index > arr->size) {
		if (index != 0) {
			TSEE_Warn("Attempted insert into array (size %zu) at index `%zu`\n",
					  arr->size, index);
			return false;
		}
	}
	TSEE_Array_Extend(arr, 1);
	for (size_t i = arr->size - 1; i > index; i--) {
		arr->data[i] = arr->data[i - 1];
	}
	// xmemmove(arr->data[index], arr->data[index + 1], sizeof(*arr->data) *
	// (arr->size - 1 - index));
	arr->data[index] = data;
	return true;
}

/**
 * @brief Deletes an item at the given index from an array.
 *
 * @todo Find out why memmove was segfaulting, for extra performance.
 *
 * @param arr Array to delete from
 * @param index Index of the item to delete.
 * @return success status
 */
bool TSEE_Array_Delete(TSEE_Array *arr, size_t index) {
	if (index >= arr->size) {
		TSEE_Warn("Attempted delete from array (size %zu) at index `%zu`\n",
				  arr->size, index);
		return false;
	}
	for (size_t i = index; i < arr->size - 1; i++) {
		arr->data[i] = arr->data[i + 1];
	}
	// xmemmove(arr->data[index + 1], arr->data[index], sizeof(*arr->data) *
	// (arr->size - index));
	arr->data = xrealloc(arr->data, sizeof(*arr->data) * (--arr->size));
	if (arr->size == 0 && arr->data) {
		xfree(arr->data);
		arr->data = NULL;
	}
	return true;
}

/**
 * @brief Deletes an item from an array.
 *
 * @param arr Array to delete from
 * @param data Data to delete.
 * @return success status.
 */
bool TSEE_Array_DeleteItem(TSEE_Array *arr, void *data) {
	int idx = TSEE_Array_GetIndex(arr, data);
	if (idx == -1) {
		TSEE_Warn("Attempted to delete non-existant item from array.\n");
		return false;
	}
	return TSEE_Array_Delete(arr, idx);
}

/**
 * @brief Gets the item at the given index from an array.
 *
 * @param arr Array to get the item from
 * @param index Index of the item
 * @return void* or NULL if its not found.
 */
void *TSEE_Array_Get(TSEE_Array *arr, size_t index) {
	if (index >= arr->size) {
		TSEE_Warn("Attempted get from array (size %zu) at index `%zu`\n",
				  arr->size, index);
		return NULL;
	}
	if (!arr->data[index]) {
		TSEE_Warn("Data value in array at index `%zu` is NULL\n", index);
	}
	return arr->data[index];
}

/**
 * @brief Returns the index for a given item.
 *
 * @param arr Array to search
 * @param data Data to compare against
 * @return Index on success, -1 on failure.
 */
int TSEE_Array_GetIndex(TSEE_Array *arr, void *data) {
	for (size_t i = 0; i < arr->size; i++) {
		if (arr->data[i] == data) {
			return i;
		}
	}
	return -1;
}

/**
 * @brief Clears an array of all its data.
 * Note: Does not free the data.
 *
 * @param arr Array to clear
 * @return success status
 */
bool TSEE_Array_Clear(TSEE_Array *arr) {
	if (!arr)
		return false;
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
 * @return success status
 */
bool TSEE_Array_Destroy(TSEE_Array *arr) {
	if (!arr)
		return false;
	if (arr->data)
		xfree(arr->data);
	xfree(arr);
	return true;
}