// Rendering

bool TSEE_Rendering_Init(TSEE *tsee);
void TSEE_Window_UpdateSize(TSEE *tsee);
void TSEE_Window_Destroy(TSEE_Window *window);
bool TSEE_Window_SetTitle(TSEE *tsee, char *title);
bool TSEE_RenderAll(TSEE *tsee);
bool TSEE_Rendering_IsReady(TSEE *tsee);

// Image

TSEE_Texture *TSEE_Texture_Create(TSEE *tsee, char *path);
TSEE_Texture *TSEE_Texture_Find(TSEE *tsee, char *path);
void TSEE_Texture_Destroy(TSEE *tsee, TSEE_Texture *tex);

// Animation

void TSEE_Animation_Init(TSEE *tsee);