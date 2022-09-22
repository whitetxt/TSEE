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

TSEE_Image *TSEE_Resource_Image_Get(TSEE *tsee) {

}

bool TSEE_Resource_Image_Store(TSEE *tsee, TSEE_Image *img) {

}

bool TSEE_Resource_Image_Delete(TSEE *tsee, TSEE_Image *img) {

}

bool TSEE_Resource_Image_DeletePath(TSEE *tsee, char *path) {
	
}

TSEE_Texture *TSEE_Resource_Texture_Find(TSEE *tsee) {

}

bool TSEE_Resource_Texture_Store(TSEE *tsee) {

}

bool TSEE_Resource_Texture_Delete(TSEE *tsee) {

}

TSEE_Font *TSEE_Resource_Font_Get(TSEE *tsee) {

}

bool TSEE_Resource_Font_Store(TSEE *tsee) {

}

bool TSEE_Resource_Font_Delete(TSEE *tsee) {

}