// Creates an object and stores it inside the TSEE
int TSEE_Create_Object(TSEE *tsee, TSEE_Texture *texture);

// Createa a player object for a physics object
bool TSEE_Player_Create(TSEE *tsee, TSEE_Physics_Object *pobj);

// Sets the player's jump force
bool TSEE_Player_SetJumpForce(TSEE *tsee, float force);

// Sets the player's speed
bool TSEE_Player_SetSpeed(TSEE *tsee, float speed);

// Adds 2 vectors
bool TSEE_Vec2_Add(TSEE_Vec2 *final, TSEE_Vec2 *add);

// Normalises a vector
bool TSEE_Vec2_Normalise(TSEE_Vec2 *vec);

// Multiplies a vector by a scalar
bool TSEE_Vec2_Multiply(TSEE_Vec2 *vec, float mult);


// PARALLAX

// Creates a parallax background.
bool TSEE_Parallax_Create(TSEE *tsee, TSEE_Texture *texture, int distanceFromCamera);

// Renders all the parallax backgrounds.
bool TSEE_Parallax_Render(TSEE *tsee);

// Destroys a parallax object
void TSEE_Parallax_Destroy(TSEE_Parallax *para, bool destroyTexture);

// TEXT

// Initializes the fonts and loads the default font if desired.
// TSEE_Fonts use names to keep track of them, so if you want to use a font, specify its name.
bool TSEE_Text_Init(TSEE *tsee, bool loadDefault);

// Loads a font from a specified file, with a specified size and specified name.
bool TSEE_Font_Load(TSEE *tsee, char *path, int size, char *name);

// Unloads a loaded font. The font must be loaded first.
bool TSEE_Font_Unload(TSEE *tsee, char *name);

// Unloads all fonts inside a TSEE object.
bool TSEE_Font_UnloadAll(TSEE *tsee);

// Returns the TTF_Font object for the specified font name.
// Returns NULL if the no font is found.
TTF_Font *TSEE_Font_Get(TSEE *tsee, char *name);

// Creates a text object and returns it
TSEE_Text *TSEE_Text_Create(TSEE *tsee, char *fontName, char *text, SDL_Color color);

// Renders the supplied TSEE_Text object to the TSEE
bool TSEE_Text_Render(TSEE *tsee, TSEE_Text *text);

// Destroys a TSEE_Text, freeing memory.
void TSEE_Text_Destroy(TSEE_Text *text, bool destroyTexture);