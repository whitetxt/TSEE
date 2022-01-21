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
	for (size_t i = 0; i < tsee->textures->size; i++) {
		TSEE_Texture *texture = TSEEArrayGet(tsee->textures, i);
		uint64_t pathSize = strlen(texture->path);
		fwrite(&pathSize, sizeof(pathSize), 1, fp);
		fwrite(&texture->path, strlen(texture->path) * sizeof(char), 1, fp);
	}
	return true;
}