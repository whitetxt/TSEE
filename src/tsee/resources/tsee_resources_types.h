typedef struct TSEE_ResManager {
	TSEE_Array *textures;
	TSEE_Array *fonts;
} TSEE_ResManager;

// TSEE's texture wrapper, keeps track of important information for the texture.
typedef struct TSEE_Texture {
	SDL_Texture *texture;
	SDL_Rect rect;
	char *path;
} TSEE_Texture;