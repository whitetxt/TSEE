// Creates an object and stores it inside the TSEE
int TSEECreateObject(TSEE *tsee, TSEE_Texture *texture);

// Creates a physics object and stores it inside the TSEE
int TSEECreatePhysicsObject(TSEE *tsee, TSEE_Texture *texture, float mass);

// Converts a regular object into a physics object
bool TSEEConvertObjectToPhysicsObject(TSEE *tsee, TSEE_Object *obj, float mass);

// Renders an object
bool TSEERenderObject(TSEE *tsee, TSEE_Object *obj);

// Performs physics for all physics objects in a TSEE
bool TSEEPerformPhysics(TSEE *tsee);

// Performs collision for all objects in a TSEE
// Called from TSEEPerformPhysics
bool TSEEPerformCollision(TSEE *tsee);

// Sets an object's position by its index
bool TSEESetObjectPosition(TSEE *tsee, int idx, float x, float y);

// Createa a player object for a physics object
bool TSEECreatePlayer(TSEE *tsee, TSEE_Physics_Object *pobj);

// Sets the player's jump force
bool TSEESetPlayerJumpForce(TSEE *tsee, float force);

// Sets the player's speed
bool TSEESetPlayerSpeed(TSEE *tsee, float speed);

// Applies a force to a physics object
bool TSEEApplyForce(TSEE_Physics_Object *pobj, TSEE_Vec2 force);

// Adds 2 vectors
bool TSEEVec2Add(TSEE_Vec2 *final, TSEE_Vec2 *add);

// Normalises a vector
bool TSEEVec2Normalise(TSEE_Vec2 *vec);

// Multiplies a vector by a scalar
bool TSEEVec2Multiply(TSEE_Vec2 *vec, float mult);

// Destroys a TSEE_Object, freeing memory
void TSEEDestroyObject(TSEE_Object *obj, bool destroyTexture);

// Destroys a TSEE_Physics_Object, freeing memory
void TSEEDestroyPhysicsObject(TSEE_Physics_Object *pobj, bool destroyObject, bool destroyTexture);


// PARALLAX

// Creates a parallax background.
bool TSEECreateParallax(TSEE *tsee, TSEE_Texture *texture, int distanceFromCamera);

// Renders all the parallax backgrounds.
bool TSEERenderParallax(TSEE *tsee);

// Destroys a parallax object
void TSEEDestroyParallax(TSEE_Parallax *para, bool destroyTexture);

// TEXT

// Initializes the fonts and loads the default font if desired.
// TSEE_Fonts use names to keep track of them, so if you want to use a font, specify its name.
bool TSEEInitText(TSEE *tsee, bool loadDefault);

// Loads a font from a specified file, with a specified size and specified name.
bool TSEELoadFont(TSEE *tsee, char *path, int size, char *name);

// Unloads a loaded font. The font must be loaded first.
bool TSEEUnloadFont(TSEE *tsee, char *name);

// Unloads all fonts inside a TSEE object.
bool TSEEUnloadAllFonts(TSEE *tsee);

// Returns the TTF_Font object for the specified font name.
// Returns NULL if the no font is found.
TTF_Font *TSEEGetFont(TSEE *tsee, char *name);

// Creates a text object and returns it
TSEE_Text *TSEECreateText(TSEE *tsee, char *fontName, char *text, SDL_Color color);

// Renders the supplied TSEE_Text object to the TSEE
bool TSEERenderText(TSEE *tsee, TSEE_Text *text);

// Destroys a TSEE_Text, freeing memory.
void TSEEDestroyText(TSEE_Text *text, bool destroyTexture);