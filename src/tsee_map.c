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
	TSEELog("Loading into %s by %s\nVersion %s\n%s\n", mapName, mapAuthor, mapVersion, mapDescription);
	// Read the gravity (for some reason its here in the header?)
	fread(&tsee->world->gravity, sizeof(tsee->world->gravity), 1, fp);
	int numTextures = 0;
	// Read number of textures for the map
	fread(&numTextures, sizeof(numTextures), 1, fp);
	// Read type of texture storage
	// 0 = Texture path stored
	// 1 = Texture stored in file
	Uint8 textureStorageType = -1;
	fread(&textureStorageType, sizeof(textureStorageType), 1, fp);
	if (textureStorageType == 0) {
		for (int i = 0; i < numTextures; i++) {
			uint64_t pathSize = 0;
			fread(&pathSize, sizeof(pathSize), 1, fp);
			char *texturePath = malloc(sizeof(*texturePath) * pathSize);
			fread(&texturePath, sizeof(texturePath), 1, fp);
			TSEE_Texture *texture = TSEECreateTexture(tsee, texturePath);
			if (!texture) {
				TSEEError("Failed to load texture `%s` (%s)\n", texturePath, SDL_GetError());
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
				TSEEError("Failed to create SDL_RWops from map file (%s)\n", SDL_GetError());
				return false;
			}
			// Read the surface and convert to a texture
			surface = IMG_LoadPNG_RW(stream);
			SDL_RWclose(stream);
			texture = SDL_CreateTextureFromSurface(tsee->window->renderer, surface);
			SDL_FreeSurface(surface);
			if (!texture) { // If the texture could not be created
				TSEEError("Failed to create texture from surface (%s)\n", SDL_GetError());
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

bool saveMap(TSEE *tsee, char *fn) {
	FILE *fp = fopen(fn, "wb");
	if (fp == NULL) {
		TSEEError("Failed to open map file (%s)\n", fn);
		return false;
	}
	// Write the map header
	char mapName[32] = "Test Map";
	char mapAuthor[64] = "Test Author";
	char mapVersion[8] = "1.0";
	char mapDescription[512] = "Test Description";
	fwrite(&mapName, sizeof(mapName), 1, fp);
	fwrite(&mapAuthor, sizeof(mapAuthor), 1, fp);
	fwrite(&mapVersion, sizeof(mapVersion), 1, fp);
	fwrite(&mapDescription, sizeof(mapDescription), 1, fp);
	// Write the gravity
	fwrite(&tsee->world->gravity, sizeof(tsee->world->gravity), 1, fp);
	// Write the number of textures
	int numTextures = tsee->textures->size;
	fwrite(&numTextures, sizeof(numTextures), 1, fp);
	// Write type of texture storage
	// 0 = Texture path stored
	// 1 = Texture stored in file
	Uint8 textureStorageType = 1;
	fwrite(&textureStorageType, sizeof(textureStorageType), 1, fp);
	// Write the textures
	if (textureStorageType == 0) {
		return false;
	} else if (textureStorageType == 1) {
		SDL_Texture* target = SDL_GetRenderTarget(tsee->window->renderer);
		for (int i = 0; i < numTextures; i++) {
			TSEE_Texture *texture = TSEEArrayGet(tsee->textures, i);
			// Get the size of the texture
			SDL_SetRenderTarget(tsee->window->renderer, texture);

			SDL_Surface *surface = SDL_CreateRGBSurface(0, texture->rect.w, texture->rect.h, 32, 0, 0, 0, 0);
			SDL_RenderReadPixels(tsee->window->renderer, NULL, surface->format->format, surface->pixels, surface->pitch);
			IMG_SavePNG();
			fwrite(&textureSize, sizeof(textureSize), 1, fp);
			// Open up a stream for SDL to read the surface from
			SDL_RWops *stream = SDL_RWFromMem(fp, textureSize);
			if (!stream) { // If the stream couldn't be opened
				TSEEError("Failed to create SDL_RWops from map file (%s)\n", SDL_GetError());
				return false;
			}
			// Read the surface and convert to a texture
			SDL_SavePNG_RW(surface, stream, 0);
			SDL_FreeSurface(surface);
			SDL_RWclose(stream);
		}
		SDL_SetRenderTarget(tsee->window->renderer, target);
	}
	return true;
};