#include "../tsee.h"

bool TSEE_Resource_Init(TSEE *tsee) {
	tsee->resources = xmalloc(sizeof(*tsee->resources));
	if (!tsee->resources) {
		TSEE_Error("Failed to malloc for resources.\n");
		return false;
	}
	tsee->resources->images = TSEE_Array_Create();
	tsee->resources->textures = TSEE_Array_Create();
	tsee->resources->fonts = TSEE_Array_Create();
	return true;
}

bool TSEE_Resource_Unload(TSEE *tsee) {
	if (!tsee->resources) return false;
}

bool TSEE_Resource_Clear(TSEE *tsee) {
	if (!tsee->resources) return false;
}

TSEE_Image *TSEE_Resource_Image_Get(TSEE *tsee, char *path) {
	for (size_t i = 0; i < tsee->resources->images->size; i++) {
		TSEE_Image *image = TSEE_Array_Get(tsee->resources->images, i);
		if (strcmp(path, image->path) == 0) {
			return image;
		}
	}
	return NULL;
}

bool TSEE_Resource_Image_Store(TSEE *tsee, TSEE_Image *img) {
	return TSEE_Array_Append(tsee->resources->images, img);
}

bool TSEE_Resource_Image_Delete(TSEE *tsee, TSEE_Image *img) {
	return TSEE_Array_DeleteItem(tsee->resources->images, img);
}

bool TSEE_Resource_Image_DeletePath(TSEE *tsee, char *path) {
	TSEE_Image *img = TSEE_Resource_Image_Get(tsee, path);
	if (!img) {
		TSEE_Warn("Attempted to delete non-existant resource `%s`\n", path);
		return false;
	}
	int idx = TSEE_Array_GetIndex(tsee->resources->images, img);
	if (idx == -1) {
		TSEE_Warn("Attempted to delete non-existant resource `%s`\n", path);
		return false;
	}
	TSEE_Array_Delete(tsee->resources->images, idx);
	return true;
}

TSEE_Texture *TSEE_Resource_Texture_Find(TSEE *tsee) {

}

bool TSEE_Resource_Texture_Store(TSEE *tsee) {

}

bool TSEE_Resource_Texture_Delete(TSEE *tsee) {

}

TSEE_Font *TSEE_Resource_Font_Get(TSEE *tsee, char *name) {
	for (size_t i = 0; i < tsee->resources->fonts->size; i++) {
		TSEE_Font *foundFont = TSEE_Array_Get(tsee->resources->fonts, i);
		if (strcmp(name, foundFont->name) == 0) {
			return foundFont;
		}
	}
	return NULL;
}

bool TSEE_Resource_Font_Store(TSEE *tsee) {

}

bool TSEE_Resource_Font_Delete(TSEE *tsee) {

}