bool TSEE_Resource_Init(TSEE *tsee);
bool TSEE_Resource_Unload(TSEE *tsee);

TSEE_Texture *TSEE_Resource_Texture_Get(TSEE *tsee, char *path);
bool TSEE_Resource_Texture_Store(TSEE *tsee, TSEE_Texture *img);
bool TSEE_Resource_Texture_Delete(TSEE *tsee, TSEE_Texture *img);
bool TSEE_Resource_Texture_DeletePath(TSEE *tsee, char *path);

TSEE_Font *TSEE_Resource_Font_Get(TSEE *tsee, char *name);
bool TSEE_Resource_Font_Store(TSEE *tsee, TSEE_Font *font);
bool TSEE_Resource_Font_Delete(TSEE *tsee, TSEE_Font *font);