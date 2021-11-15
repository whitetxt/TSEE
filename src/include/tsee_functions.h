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

// Extends a TSEE_Array by 1.
int TSEEArrayExtend(TSEE_Array *arr);

// Appends data to the end of a TSEE_Array.
bool TSEEArrayAppend(TSEE_Array *arr, void *data);

// Insert data into TSEE_Array at the specified index.
bool TSEEArrayInsert(TSEE_Array *arr, void *toInsert, int index);

// Deletes data from a TSEE_Array at the specified index.
bool TSEEArrayDelete(TSEE_Array *arr, int index);

// !! tsee_logging.c !!
// Log something to the console, typically for debugging.
void TSEELog(char *message, ...);

// Inform the user of a warning, typically used for something important, but not a full error.
void TSEEWarn(char *message, ...);

// Inform the user of an error in the console, typically used for something being invalid, but not a full crash.
void TSEEError(char *message, ...);

// Inform the user of a critical error, should only be used if the program can't continue.
void TSEECritical(char *message, ...);