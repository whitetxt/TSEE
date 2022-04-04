// Initializes the rendering component of TSEE, and creates a window + renderer.
bool TSEE_Rendering_Init(TSEE *tsee);

// Updates the window size for a TSEE object.
void TSEE_Window_UpdateSize(TSEE *tsee);

// Destroy a TSEE_Window, NOT FOR USE: USE TSEE_Close for closing the entire TSEE instance
void TSEE_Window_Destroy(TSEE_Window *window);

// Sets a TSEE object's window title.
bool TSEE_Window_SetTitle(TSEE *tsee, char *title);

// Renders all objects in a TSEE to the TSEE's window.
bool TSEE_RenderAll(TSEE *tsee);

// Determines if we are ready to render or not.
// If returns true, time since last render is >= than the fps.
// If returns false, time since last render is < than the fps.
bool TSEE_Rendering_IsReady(TSEE *tsee);

// IMAGE

// Creates a TSEE_Texture from an file path.
TSEE_Texture *TSEE_Texture_Create(TSEE *tsee, char *path);

// Finds an already loaded TSEE_Texture from a file path.
TSEE_Texture *TSEE_Texture_Find(TSEE *tsee, char *path);

// Frees a TSEE_Texture.
void TSEE_Texture_Destroy(TSEE_Texture *tex);

// ANIMATION

void TSEE_Animation_Init(TSEE *tsee);