// !! tsee.c !!

// Creates a new tsee object.
TSEE TSEECreate(int width, int height);

// !! tsee_rendering.c !!

// Initialises the rendering component of TSEE, and creates a window + renderer.
bool TSEEInitRendering(TSEE *tsee);

// Destroy a TSEE_Window, NOT FOR USE: USE TSEEClose for closing the entire TSEE instance
void TSEEDestroyWindow(TSEE_Window *window);

// !! tsee_objects.c !!

// !! tsee_logging.c !!
// Log something to the console, typically for debugging.
void TSEELog(char *message, ...);

// Inform the user of a warning, typically used for something important, but not a full error.
void TSEEWarn(char *message, ...);

// Inform the user of an error in the console, typically used for something being invalid, but not a full crash.
void TSEEError(char *message, ...);

// Inform the user of a critical error, should only be used if the program can't continue.
void TSEECritical(char *message, ...);