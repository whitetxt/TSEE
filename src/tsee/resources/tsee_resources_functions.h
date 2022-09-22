bool TSEE_Resource_Init(TSEE *tsee);
bool TSEE_Resource_Unload(TSEE *tsee);
bool TSEE_Resource_Clear(TSEE *tsee);

TSEE_Texture *TSEE_Resource_Texture_Find();
bool TSEE_Resource_Texture_Store();
bool TSEE_Resource_Texture_Delete();

TSEE_Font *TSEE_Resource_Font_Get();
bool TSEE_Resource_Font_Store();
bool TSEE_Resource_Font_Delete();