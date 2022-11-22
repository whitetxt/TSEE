// Objects

bool TSEE_Object_CheckAttribute(TSEE_Object *obj, TSEE_Object_Attributes attr);
TSEE_Object *TSEE_Object_Create(TSEE *tsee,
								TSEE_Texture *texture,
								TSEE_Object_Attributes attributes,
								float x,
								float y);
bool TSEE_Player_Create(TSEE *tsee, TSEE_Object *pobj);
bool TSEE_Object_SetPosition(TSEE *tsee, TSEE_Object *obj, float x, float y);
bool TSEE_Object_SetPositionVec2(TSEE *tsee, TSEE_Object *obj, TSEE_Vec2 vec);
SDL_Rect TSEE_Object_GetCollisionRect(TSEE_Object *obj, TSEE_Object *other);
SDL_Rect TSEE_Object_GetRect(TSEE_Object *obj);
bool TSEE_Object_Render(TSEE *tsee, TSEE_Object *object);
void TSEE_Object_Destroy(TSEE *tsee, TSEE_Object *object, bool destroyTexture);

// Attributes

bool TSEE_Attributes_Check(TSEE_Object_Attributes attr,
						   TSEE_Object_Attributes attr2);
void TSEE_Attributes_Set(TSEE_Object_Attributes *attr,
						 TSEE_Object_Attributes to_set);

// Parallax

TSEE_Object *TSEE_Parallax_Create(TSEE *tsee,
								  TSEE_Texture *texture,
								  float distanceFromCamera);
TSEE_Object *TSEE_Parallax_CreateFromObject(TSEE *tsee,
											TSEE_Object *obj,
											float distanceFromCamera);
bool TSEE_Parallax_Render(TSEE *tsee, TSEE_Object *parallax);
void TSEE_Parallax_Destroy(TSEE *tsee, TSEE_Object *para, bool destroyTexture);

// Text

bool TSEE_Text_Init(TSEE *tsee, bool loadDefault);
bool TSEE_Font_Load(TSEE *tsee, char *path, int size, char *name);
bool TSEE_Font_Unload(TSEE *tsee, TSEE_Font *font);
bool TSEE_Font_UnloadName(TSEE *tsee, char *name);
bool TSEE_Font_UnloadAll(TSEE *tsee);
TSEE_Font *TSEE_Font_Get(TSEE *tsee, char *name);
TSEE_Object *TSEE_Text_Create(TSEE *tsee,
							  char *fontName,
							  char *text,
							  SDL_Color color);
bool TSEE_Text_Render(TSEE *tsee, TSEE_Object *text);
void TSEE_Text_Destroy(TSEE *tsee, TSEE_Object *text, bool destroyTexture);