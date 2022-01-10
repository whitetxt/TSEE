#include "include/tsee.h"

bool TSEELoadMap(TSEE *tsee, char *fn) {
	FILE *fp = fopen(fn, "rb");
	if (fp == NULL) {
		TSEEError("Failed to open map file (%s)\n", fn);
		return false;
	}
	// Clear arrays to be overwritten
	TSEEArrayClear(tsee->world->objects);
	TSEEArrayClear(tsee->world->physics_objects);
	TSEEArrayClear(tsee->world->parallax);
	TSEEArrayClear(tsee->world->text);
	char mapName[32];
	char mapAuthor[64];
	char mapVersion[8];
	char mapDescription[512];
	// Read all of the map header data
	fread(&mapName, sizeof(mapName), 1, fp);
	fread(&mapAuthor, sizeof(mapAuthor), 1, fp);
	fread(&mapVersion, sizeof(mapVersion), 1, fp);
	fread(&mapDescription, sizeof(mapDescription), 1, fp);
	printf("Loading into %s by %s\nVersion %s\n%s\n", mapName, mapAuthor, mapVersion, mapDescription);
	// Read the gravity (for some reason its here in the header?)
	fread(&tsee->world->gravity, sizeof(tsee->world->gravity), 1, fp);
	int numTextures = 0;
	// Read number of textures for the map
	fread(&numTextures, sizeof(numTextures), 1, fp);
	// Read type of texture storage
	// 0 = Texture path stored
	// 1 = Texture stored in file
	int textureStorageType = -1;
	fread(&textureStorageType, sizeof(textureStorageType), 1, fp);
	if (textureStorageType == 0) {
		for (int i = 0; i < numTextures; i++) {
			uint64_t pathSize = 0;
			fread(&pathSize, sizeof(pathSize), 1, fp);
			char *texturePath = malloc(sizeof(*texturePath) * pathSize);
			fread(&texturePath, sizeof(texturePath), 1, fp);
			TSEE_Texture *texture = TSEECreateTextureFromPath(tsee, texturePath);
			if (!texture) {
				TSEEError("Failed to load texture (%s)\n%s\n", texturePath, SDL_GetError());
				return false;
			}
			TSEEArrayAppend(tsee->textures, texture);
		}
	} else if (textureStorageType == 1) {
		for (int i = 0; i < numTextures; i++) {
			// Get the size of the texture
			uint64_t textureSize = 0;
			fread(&textureSize, sizeof(textureSize), 1, fp);
			SDL_Texture *texture = malloc(sizeof(texture));
			SDL_Surface *surface = NULL;
			// Open up a stream for SDL to read the surface from
			SDL_RWops *stream = SDL_RWFromMem(fp, textureSize);
			if (!stream) { // If the stream couldn't be opened
				TSEEError("Failed to create SDL_RWops from map file:\n%s\n", SDL_GetError());
				return false;
			}
			// Read the surface and convert to a texture
			surface = IMG_LoadPNG_RW(stream);
			SDL_RWclose(stream);
			texture = SDL_CreateTextureFromSurface(tsee->window->renderer, surface);
			SDL_FreeSurface(surface);
			if (!texture) { // If the texture could not be created
				TSEEError("Failed to create texture from surface:\n%s\n", SDL_GetError());
				return false;
			}
			TSEEArrayAppend(tsee->textures, texture);
		}
	}
	// Read map's maximum scroll
	fread(&tsee->world->max_scroll_x, sizeof(tsee->world->max_scroll_x), 1, fp);
	
	// Read map's parallax layers
	int numParallax = 0;
	fread(&numParallax, sizeof(numParallax), 1, fp);
	for (int i = 0; i < numParallax; i++) {
		TSEE_Parallax *parallax = malloc(sizeof(*parallax));
		int textureIdx = -1;
		fread(&textureIdx, sizeof(textureIdx), 1, fp);
		parallax->texture = TSEEArrayGet(tsee->textures, textureIdx);
		fread(&parallax->distance, sizeof(parallax->distance), 1, fp);
		bool inserted = false;
		for (size_t j = 0; j < tsee->world->parallax->size; j++) {
			TSEE_Parallax *parallax2 = TSEEArrayGet(tsee->world->parallax, j);
			if (parallax->distance > parallax2->distance) {
				TSEEArrayInsert(tsee->world->parallax, parallax, j);
				inserted = true;
				break;
			}
		}
		if (!inserted) {
			TSEEArrayAppend(tsee->world->parallax, parallax);
		}
	}
	return true;
}