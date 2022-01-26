// !! Logging functions !!

// Log something to the console, typically for debugging.
#define TSEELog(message, ...) \
		fprintf(stdout, "TSEE [log] (%s): " message "", __func__, ##__VA_ARGS__)

// Inform the user of a warning, typically used for something important, but not a full error.
#define TSEEWarn(message, ...) \
		fprintf(stdout, "TSEE [warn] (%s): " message "", __func__, ##__VA_ARGS__)

// Inform the user of an error in the console, typically used for something being invalid, but not a full crash.
#define TSEEError(message, ...) \
		fprintf(stderr, "TSEE [error] (%s): " message "", __func__, ##__VA_ARGS__)

// Inform the user of a critical error, should only be used if the program can't continue.
#define TSEECritical(message, ...) \
		fprintf(stderr, "TSEE [!! Critical !!] (%s): " message "", __func__, ##__VA_ARGS__)

// !! tsee.c !!

// Creates a new TSEE object.
TSEE TSEECreate(int width, int height);

// Initializes all TSEE components.
bool TSEEInitAll(TSEE *tsee);

// Closes a TSEE object, clears all memory out and deletes it.
bool TSEEClose(TSEE *tsee);

// Calculates the delta time for a TSEE
bool TSEECalculateDT(TSEE *tsee);

// Sets the world's gravity
bool TSEESetWorldGravity(TSEE *tsee, double gravity);

// !! tsee_rendering.c !!

// Initializes the rendering component of TSEE, and creates a window + renderer.
bool TSEEInitRendering(TSEE *tsee);

// Updates the window size for a TSEE object.
void TSEEWindowUpdateSize(TSEE *tsee);

// Destroy a TSEE_Window, NOT FOR USE: USE TSEEClose for closing the entire TSEE instance
void TSEEDestroyWindow(TSEE_Window *window);

// Sets a TSEE object's window title.
bool TSEESetWindowTitle(TSEE *tsee, char *title);

// Renders all objects in a TSEE to the TSEE's window.
bool TSEERenderAll(TSEE *tsee);

// !! tsee_objects.c !!

// Creates an object and stores it inside the TSEE
int TSEECreateObject(TSEE *tsee, TSEE_Texture *texture);

// Creates a physics object and stores it inside the TSEE
int TSEECreatePhysicsObject(TSEE *tsee, TSEE_Texture *texture, double mass);

// Converts a regular object into a physics object
bool TSEEConvertObjectToPhysicsObject(TSEE *tsee, TSEE_Object *obj, double mass);

// Renders an object
bool TSEERenderObject(TSEE *tsee, TSEE_Object *obj);

// Performs physics for all physics objects in a TSEE
bool TSEEPerformPhysics(TSEE *tsee);

// Performs collision for all objects in a TSEE
// Called from TSEEPerformPhysics
bool TSEEPerformCollision(TSEE *tsee);

// Sets an object's position by its index
bool TSEESetObjectPosition(TSEE *tsee, int idx, double x, double y);

// Createa a player object for a physics object
bool TSEECreatePlayer(TSEE *tsee, TSEE_Physics_Object *pobj);

// Sets the player's jump force
bool TSEESetPlayerJumpForce(TSEE *tsee, double force);

// Sets the player's speed
bool TSEESetPlayerSpeed(TSEE *tsee, double speed);

// Applies a force to a physics object
bool TSEEApplyForce(TSEE_Physics_Object *pobj, TSEE_Vec2 force);

// Adds 2 vectors
bool TSEEVec2Add(TSEE_Vec2 *final, TSEE_Vec2 *add);

// Normalises a vector
bool TSEEVec2Normalise(TSEE_Vec2 *vec);

// Multiplies a vector by a scalar
bool TSEEVec2Multiply(TSEE_Vec2 *vec, double mult);

// !! tsee_array.c !!

// Creates a TSEE_Array with size 0.
TSEE_Array *TSEEArrayCreate();

// Extends a TSEE_Array by the specified size.
int TSEEArrayExtend(TSEE_Array *arr, int size);

// Appends data to the end of a TSEE_Array.
bool TSEEArrayAppend(TSEE_Array *arr, void *data);

// Insert data into TSEE_Array at the specified index.
bool TSEEArrayInsert(TSEE_Array *arr, void *toInsert, size_t index);

// Deletes data from a TSEE_Array at the specified index.
// This will move all following data to the left.
// Update any stored indexes after using this!
bool TSEEArrayDelete(TSEE_Array *arr, size_t index);

// Gets the specified index from a TSEE_Array.
void *TSEEArrayGet(TSEE_Array *arr, size_t index);

// Clears a TSEE_Array, deleting all data and setting size to 0.
bool TSEEArrayClear(TSEE_Array *arr);

// Frees a TSEE_Array, freeing all memory.
bool TSEEArrayFree(TSEE_Array *arr);

// !! tsee_text.c !!

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

// !! tsee_settings.c !!

// Callback for TSEE's settings loading system. Do not use.
void TSEESettingsLoadCallback(TSEE *tsee, char *section, char *value);

// Loads TSEE's settings from a file.
// THIS WILL NOT LOAD YOUR OWN SETTINGS!!
bool TSEELoadSettings(TSEE *tsee);

// Saves TSEE's settings to a file.
// THIS WILL NOT SAVE YOUR OWN SETTINGS!!
bool TSEESaveSettings(TSEE *tsee);

// !! tsee_image.c !!

// Creates a TSEE_Texture from an file path.
TSEE_Texture *TSEECreateTexture(TSEE *tsee, char *path);

// Finds an already loaded TSEE_Texture from a file path.
TSEE_Texture *TSEEFindTexture(TSEE *tsee, char *path);

// !! tsee_events.c !!

// Initializes events for a TSEE
bool TSEEInitEvents(TSEE *tsee);

// Handles events for a TSEE and calls the callbacks for each event.
bool TSEEHandleEvents(TSEE *tsee);

// !! tsee_input.c !!

// Initializes TSEE input and includes some basic functions for movement of the player.
bool TSEEInitInput(TSEE *tsee);

// Basic key press and release function.
void TSEEKeyPress(void *tsee, SDL_Keycode key);

// Basic key press and release function.
void TSEEKeyRelease(void *tsee, SDL_Keycode key);

// !! tsee_parallax.c !!

// Creates a parallax background.
bool TSEECreateParallax(TSEE *tsee, TSEE_Texture *texture, int distanceFromCamera);

// Renders all the parallax backgrounds.
bool TSEERenderParallax(TSEE *tsee);

// !! tsee_ui.c !!

// Initializes the UI system.
// MUST be called after TSEEInitText
bool TSEEInitUI(TSEE *tsee);

// Adds a button to the top-level toolbar.
bool TSEEAddToolbarButton(TSEE *tsee, char *font, char *text);

// Adds a child button to a parent button.
bool TSEEAddToolbarChild(TSEE *tsee, TSEE_Toolbar_Object *parent, char *font, char *text, void (*cb) (void *tsee));

// Checks if the UI was clicked
bool TSEEUIClick(TSEE *tsee, int x, int y);

// Renders all toolbar buttons.
bool TSEERenderUI(TSEE *tsee);

// !! tsee_map.c !!

// Internal helper function to read from a file until it hits a null char.
bool readFileUntilNull(FILE *fp, char *buffer);

// Loads a map from a file.
bool TSEELoadMap(TSEE *tsee, char *path);

// Saves a map to a file.
bool TSEESaveMap(TSEE *tsee, char *path);