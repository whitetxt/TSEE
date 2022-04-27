// Objects

bool TSEE_Object_CheckAttribute(TSEE_Object *obj, TSEE_Object_Attributes attr);
TSEE_Object *TSEE_Object_Create(TSEE *tsee, TSEE_Texture *texture, TSEE_Object_Attributes attributes, float x, float y);
bool TSEE_Player_Create(TSEE *tsee, TSEE_Object *pobj);
bool TSEE_Object_SetPosition(TSEE_Object *obj, float x, float y);
SDL_Rect TSEE_Object_GetCollisionRect(TSEE_Object *obj, TSEE_Object *other);
bool TSEE_Object_Render(TSEE *tsee, TSEE_Object *object);
void TSEE_Object_Destroy(TSEE_Object *object, bool destroyTexture);

// Attributes

bool TSEE_Attributes_Check(TSEE_Object_Attributes attr, TSEE_Object_Attributes attr2);
void TSEE_Attributes_Set(TSEE_Object_Attributes *attr, TSEE_Object_Attributes to_set);

// Parallax

bool TSEE_Parallax_Create(TSEE *tsee, TSEE_Texture *texture, float distanceFromCamera);
bool TSEE_Parallax_CreateFromObject(TSEE *tsee, TSEE_Object *obj, float distanceFromCamera);
bool TSEE_Parallax_Render(TSEE *tsee, TSEE_Object *parallax);
void TSEE_Parallax_Destroy(TSEE_Object *para, bool destroyTexture);

// Text

bool TSEE_Object_Init(TSEE *tsee, bool loadDefault);
bool TSEE_Font_Load(TSEE *tsee, char *path, int size, char *name);
bool TSEE_Font_Unload(TSEE *tsee, char *name);
bool TSEE_Font_UnloadAll(TSEE *tsee);
TTF_Font *TSEE_Font_Get(TSEE *tsee, char *name);
TSEE_Object *TSEE_Text_Create(TSEE *tsee, char *fontName, char *text, SDL_Color color);
bool TSEE_Text_Render(TSEE *tsee, TSEE_Object *text);
void TSEE_Text_Destroy(TSEE_Object *text, bool destroyTexture);