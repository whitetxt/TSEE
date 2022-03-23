// !! Logging functions !!

#ifdef TSEE_DEV
// Log something to the console, typically for debugging.
#define TSEELog(message, ...) \
		fprintf(stdout, "TSEE [Log] (%s:%d): " message "", __FILE__, __LINE__, ##__VA_ARGS__)

// Inform the user of a warning, typically used for something important, but not a full error.
#define TSEEWarn(message, ...) \
		fprintf(stdout, "TSEE [Warn] (%s:%d): " message "", __FILE__, __LINE__, ##__VA_ARGS__)

// Inform the user of an error in the console, typically used for something being invalid, but not a full crash.
#define TSEEError(message, ...) \
		fprintf(stdout, "TSEE [Error] (%s:%d): " message "", __FILE__, __LINE__, ##__VA_ARGS__)

// Inform the user of a critical error, should only be used if the program can't continue.
#define TSEECritical(message, ...) \
		fprintf(stdout, "TSEE [!! CRITICAL !!] (%s:%d): " message "", __FILE__, __LINE__, ##__VA_ARGS__)

#else

// Log something to the console, typically for debugging.
#define TSEELog(message, ...) \
		fprintf(stdout, "TSEE [Log] (%s): " message "", __func__, ##__VA_ARGS__)

// Inform the user of a warning, typically used for something important, but not a full error.
#define TSEEWarn(message, ...) \
		fprintf(stdout, "TSEE [Warn] (%s): " message "", __func__, ##__VA_ARGS__)

// Inform the user of an error in the console, typically used for something being invalid, but not a full crash.
#define TSEEError(message, ...) \
		fprintf(stdout, "TSEE [Error] (%s): " message "", __func__, ##__VA_ARGS__)

// Inform the user of a critical error, should only be used if the program can't continue.
#define TSEECritical(message, ...) \
		fprintf(stdout, "TSEE [!! CRITICAL !!] (%s): " message "", __func__, ##__VA_ARGS__)

#endif

#define xfree(ptr) \
		_xfree(ptr, __FILE__, __LINE__);

#define xmalloc(size) \
		_xmalloc(size, __FILE__, __LINE__);

#define xrealloc(ptr, size) \
		_xrealloc(ptr, size, __FILE__, __LINE__);

#define xmemmove(dst, src, size) \
		_xmemmove(dst, src, size, __FILE__, __LINE__);

// !! tsee.c !!

// Creates a new TSEE object.
TSEE *TSEECreate(int width, int height);

// Initializes all TSEE components.
bool TSEEInitAll(TSEE *tsee);

// Closes a TSEE object, clears all memory out and deletes it.
bool TSEEClose(TSEE *tsee);

// Calculates the delta time for a TSEE
bool TSEECalculateDT(TSEE *tsee);

// Sets the world's gravity
bool TSEESetWorldGravity(TSEE *tsee, float gravity);

// Determines if we are ready to render or not.
// If returns true, time since last render is >= than the fps.
// If returns false, time since last render is < than the fps.
bool TSEEReadyToRender(TSEE *tsee);

// !! tsee_settings.c !!

// Callback for TSEE's settings loading system. Do not use.
void TSEESettingsLoadCallback(TSEE *tsee, char *section, char *value);

// Loads TSEE's settings from a file.
// THIS WILL NOT LOAD YOUR OWN SETTINGS!!
bool TSEELoadSettings(TSEE *tsee);

// Saves TSEE's settings to a file.
// THIS WILL NOT SAVE YOUR OWN SETTINGS!!
bool TSEESaveSettings(TSEE *tsee);