// !! tsee.c !!

// Creates a new TSEE object.
TSEE TSEECreate(int width, int height);

// Closes a TSEE object, clears all memory out and deletes it.
bool TSEEClose(TSEE *tsee);

// !! tsee_rendering.c !!

// Initialises the rendering component of TSEE, and creates a window + renderer.
bool TSEEInitRendering(TSEE *tsee);

// Destroy a TSEE_Window, NOT FOR USE: USE TSEEClose for closing the entire TSEE instance
void TSEEDestroyWindow(TSEE_Window *window);

// !! tsee_objects.c !!

// !! tsee_array.c !!

// Creates a TSEE_Array with size 0.
TSEE_Array *TSEEArrayCreate();

// Extends a TSEE_Array by 1.
int TSEEArrayExtend(TSEE_Array *arr);

// Appends data to the end of a TSEE_Array.
bool TSEEArrayAppend(TSEE_Array *arr, void *data);

// Insert data into TSEE_Array at the specified index.
bool TSEEArrayInsert(TSEE_Array *arr, void *toInsert, size_t index);

// Deletes data from a TSEE_Array at the specified index.
bool TSEEArrayDelete(TSEE_Array *arr, size_t index);

// Gets the specified index from a TSEE_Array.
void *TSEEArrayGet(TSEE_Array *arr, size_t index);

// Clears a TSEE_Array, freeing all memory.
bool TSEEArrayClear(TSEE_Array *arr);

// !! tsee_logging.c !!
// Log something to the console, typically for debugging.
void TSEELog(char *message, ...);

// Inform the user of a warning, typically used for something important, but not a full error.
void TSEEWarn(char *message, ...);

// Inform the user of an error in the console, typically used for something being invalid, but not a full crash.
void TSEEError(char *message, ...);

// Inform the user of a critical error, should only be used if the program can't continue.
void TSEECritical(char *message, ...);

// !! tsee_text.c !!

// Initialises the fonts and loads the default font if desired.
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

// !! tsee_settings.c !!

// Callback for TSEE's settings loading system. Do not use.
void TSEESettingsLoadCallback(TSEE *tsee, char *section, char *value);

// Loads TSEE's settings from a file.
// THIS WILL NOT LOAD YOUR OWN SETTINGS!!
bool TSEELoadSettings(TSEE *tsee);

// Saves TSEE's settings to a file.
// THIS WILL NOT SAVE YOUR OWN SETTINGS!!
bool TSEESaveSettings(TSEE *tsee);