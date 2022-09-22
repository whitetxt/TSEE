typedef struct TSEE_ResManager {
	TSEE_Array *images;
	TSEE_Array *textures;
	TSEE_Array *fonts;
} TSEE_ResManager;

typedef struct TSEE_Image {
	SDL_Texture *texture;
	char *path;
} TSEE_Image;

// TSEE's texture wrapper, keeps track of important information for the texture.
typedef struct TSEE_Texture {
	TSEE_Image *image;
	SDL_Rect rect;
} TSEE_Texture;
