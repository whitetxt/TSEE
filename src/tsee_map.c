#include "include/tsee.h"

bool readFileUntilNull(FILE *fp, char *buffer) {
	char c;
	int len = 0;
	while ((c = getc(fp)) != 0) {
		char *newPtr = realloc(buffer, sizeof(*buffer) * (++len));
		if (!newPtr) {
			TSEECritical("Failed realloc for text buffer.\n");
			return false;
		}
		buffer[len - 1] = c;
	}
	buffer[len] = '\0';
	return true;
}

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
	// Read all of the map header data
	char *mapName = NULL;
	if (!readFileUntilNull(fp, mapName)) return false;
	char *mapAuthor = NULL;
	if (!readFileUntilNull(fp, mapAuthor)) return false;
	char *mapVersion = NULL;
	if(!readFileUntilNull(fp, mapVersion)) return false;
	char *mapDescription = NULL;
	if(!readFileUntilNull(fp, mapDescription)) return false;
	TSEELog("Loading into %s by %s\nVersion: %s\n%s\n", mapName, mapAuthor, mapVersion, mapDescription);
	free(mapName);
	free(mapAuthor);
	free(mapVersion);
	free(mapDescription);
	// Read the gravity (for some reason its here in the header?)
	float gravity = 0;
	fread(&gravity, sizeof(gravity), 1, fp);
	tsee->world->gravity = gravity;
	size_t numTextures = 0;
	// Read number of textures for the map
	fread(&numTextures, sizeof(numTextures), 1, fp);
	for (size_t i = 0; i < numTextures; i++) {
		size_t pathSize = 0;
		fread(&pathSize, sizeof(pathSize), 1, fp);
		char *texturePath = malloc(sizeof(*texturePath) * pathSize);
		fread(&texturePath, sizeof(*texturePath) * pathSize, 1, fp);
		TSEE_Texture *texture = TSEECreateTexture(tsee, texturePath);
		if (!texture) {
			TSEEError("Failed to load texture `%s` (%s)\n", texturePath, SDL_GetError());
			return false;
		}
	}
	
	// Read map's parallax layers
	size_t numParallax = 0;
	fread(&numParallax, sizeof(numParallax), 1, fp);
	for (size_t i = 0; i < numParallax; i++) {
		TSEE_Parallax *parallax = malloc(sizeof(*parallax));
		size_t textureIdx = -1;
		fread(&textureIdx, sizeof(textureIdx), 1, fp);
		size_t dist = 0;
		fread(&dist, sizeof(dist), 1, fp);
		if (!TSEECreateParallax(tsee, TSEEArrayGet(tsee->textures, textureIdx), dist)) {
			TSEEWarn("Failed to create parallax with texture index `%zu`\n", textureIdx);
		}
	}
	return true;
}

bool TSEESaveMap(TSEE *tsee, char *fn) {
	FILE *fp = fopen(fn, "wb");
	if (fp == NULL) {
		TSEEError("Failed to open map file (%s)\n", fn);
		return false;
	}
	// Write the map header
	char *mapName = "Test Map";
	char *mapAuthor = "Test Author";
	char *mapVersion = "1.0";
	char *mapDescription = "Test Description";
	fwrite(mapName, sizeof(*mapName) * strlen(mapName), 1, fp);
	putc(0, fp);
	fwrite(mapAuthor, sizeof(*mapAuthor) * strlen(mapAuthor), 1, fp);
	putc(0, fp);
	fwrite(mapVersion, sizeof(*mapVersion) * strlen(mapVersion), 1, fp);
	putc(0, fp);
	fwrite(mapDescription, sizeof(*mapDescription) * strlen(mapDescription), 1, fp);
	putc(0, fp);
	// Write the gravity
	float gravity = tsee->world->gravity;
	fwrite(&gravity, sizeof(gravity), 1, fp);
	// Write the number of textures
	size_t numTextures = tsee->textures->size;
	fwrite(&numTextures, sizeof(numTextures), 1, fp);
	for (size_t i = 0; i < tsee->textures->size; i++) {
		TSEE_Texture *texture = TSEEArrayGet(tsee->textures, i);
		int pathSize = strlen(texture->path);
		fwrite(texture->path, pathSize * sizeof(char), 1, fp);
		putc(0, fp);
	}
	// Write the parallax backgrounds with texture indexes.
	fwrite(&tsee->world->parallax->size, sizeof(tsee->world->parallax->size), 1, fp);
	for (size_t i = 0; i < tsee->world->parallax->size; i++) {
		TSEE_Parallax *parallax = TSEEArrayGet(tsee->world->parallax, i);
		for (size_t j = 0; j < tsee->textures->size; j++) {
			TSEE_Texture *texture = TSEEArrayGet(tsee->textures, j);
			if (texture == parallax->texture) {
				size_t textureIdx = j;
				fwrite(&textureIdx, sizeof(textureIdx), 1, fp);
				break;
			}
		}
		size_t dist = parallax->distance;
		fwrite(&dist, sizeof(dist), 1, fp);
	}
	// Write the objects with texture indexes.
	fwrite(&tsee->world->objects->size, sizeof(tsee->world->objects->size), 1, fp);
	for (size_t i = 0; i < tsee->world->objects->size; i++) {
		TSEE_Object *object = TSEEArrayGet(tsee->world->objects, i);
		for (size_t j = 0; j < tsee->textures->size; j++) {
			TSEE_Texture *texture = TSEEArrayGet(tsee->textures, j);
			if (texture == object->texture) {
				uint64_t textureIdx = j;
				fwrite(&textureIdx, sizeof(textureIdx), 1, fp);
				break;
			}
		}
		fwrite(&object->x, sizeof(object->x), 1, fp);
		fwrite(&object->y, sizeof(object->y), 1, fp);
	}
	// Write the physics objects
	fwrite(&tsee->world->physics_objects->size, sizeof(tsee->world->physics_objects->size), 1, fp);
	for (size_t i = 0; i < tsee->world->physics_objects->size; i++) {
		TSEE_Physics_Object *object = TSEEArrayGet(tsee->world->physics_objects, i);
		// Find which object it relates to
		for (size_t j = 0; j < tsee->world->objects->size; j++) {
			TSEE_Object *object2 = TSEEArrayGet(tsee->world->objects, j);
			if (object2 == object->object) {
				uint64_t objectIdx = j;
				fwrite(&objectIdx, sizeof(objectIdx), 1, fp);
				break;
			}
		}
		fwrite(&object->mass, sizeof(object->mass), 1, fp);
	}
	// Write which physics object is the player
	int playerIdx = -1;
	if (tsee->player != NULL) {
		for (size_t i = 0; i < tsee->world->physics_objects->size; i++) {
			TSEE_Physics_Object *object = TSEEArrayGet(tsee->world->physics_objects, i);
			if (object == tsee->player->physics_object) {
				playerIdx = i;
				break;
			}
		}
		if (playerIdx == -1) {
			TSEEError("Failed to find player physics object in map file\n");
		}
		Sint64 towrite = playerIdx;
		fwrite(&towrite, sizeof(towrite), 1, fp);
	}
	TSEELog("Saved map to %s\n", fn);
	return true;
}