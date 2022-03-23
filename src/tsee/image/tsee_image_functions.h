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

// IMAGE

// Creates a TSEE_Texture from an file path.
TSEE_Texture *TSEECreateTexture(TSEE *tsee, char *path);

// Finds an already loaded TSEE_Texture from a file path.
TSEE_Texture *TSEEFindTexture(TSEE *tsee, char *path);

// Frees a TSEE_Texture.
void TSEEDestroyTexture(TSEE_Texture *tex);

// ANIMATION

void TSEEInitAnimation(TSEE *tsee);