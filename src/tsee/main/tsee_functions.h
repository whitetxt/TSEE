#if TSEE_DEV

// Log something to the console, typically for debugging.
#define TSEE_Log(message, ...)                                             \
	fprintf(stdout, "TSEE [Log] (%s:%d): " message "", __FILE__, __LINE__, \
			##__VA_ARGS__)

// Inform the user of a warning, typically used for something important, but not
// a full error.
#define TSEE_Warn(message, ...)                                             \
	fprintf(stdout, "TSEE [Warn] (%s:%d): " message "", __FILE__, __LINE__, \
			##__VA_ARGS__)

// Inform the user of an error in the console, typically used for something
// being invalid, but not a full crash.
#define TSEE_Error(message, ...)                                             \
	fprintf(stdout, "TSEE [Error] (%s:%d): " message "", __FILE__, __LINE__, \
			##__VA_ARGS__)

// Inform the user of a critical error, should only be used if the program can't
// continue.
#define TSEE_Critical(message, ...)                                         \
	fprintf(stdout, "TSEE [!! CRITICAL !!] (%s:%d): " message "", __FILE__, \
			__LINE__, ##__VA_ARGS__)

#else

// Log something to the console, typically for debugging.
#define TSEE_Log(message, ...)

// Inform the user of a warning, typically used for something important, but not
// a full error.
#define TSEE_Warn(message, ...)

// Inform the user of an error in the console, typically used for something
// being invalid, but not a full crash.
#define TSEE_Error(message, ...) \
	fprintf(stdout, "TSEE [Error] (%s): " message "", __func__, ##__VA_ARGS__)

// Inform the user of a critical error, should only be used if the program can't
// continue.
#define TSEE_Critical(message, ...)                                      \
	fprintf(stdout, "TSEE [!! CRITICAL !!] (%s): " message "", __func__, \
			##__VA_ARGS__)

#endif

#define xfree(ptr) _xfree(ptr, __FILE__, __LINE__)

#define xmalloc(size) _xmalloc(size, __FILE__, __LINE__)

#define xrealloc(ptr, size) _xrealloc(ptr, size, __FILE__, __LINE__)

#define xmemmove(src, dst, size) _xmemmove(src, dst, size, __FILE__, __LINE__)

// Main Functions

TSEE *TSEE_Create(int width, int height);
bool TSEE_InitAll(TSEE *tsee);
bool TSEE_Close(TSEE *tsee);
bool TSEE_CalculateDT(TSEE *tsee);
bool TSEE_World_SetGravity(TSEE *tsee, TSEE_Vec2 gravity);
void TSEE_World_ScrollToObject(TSEE *tsee, TSEE_Object *obj);
void TSEE_MainLoop(TSEE *tsee);

// Settings

void TSEE_Settings_LoadCallback(TSEE *tsee, char *section, char *value);
bool TSEE_Settings_Load(TSEE *tsee);
bool TSEE_Settings_Save(TSEE *tsee);
bool TSEE_Settings_ConvertToBool(char *value, bool *result);