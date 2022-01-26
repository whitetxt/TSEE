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

bool TSEESaveMap(TSEE *tsee, char *fn) {
	FILE *fp = fopen(fn, "wb");
	if (fp == NULL) {
		TSEEError("Failed to open map file (%s)\n", fn);
		return false;
	}
	// Write the map header
	char *mapName = "Test Map";
	size_t mapNameSize = strlen(mapName);
	char *mapAuthor = "Test Author";
	size_t mapAuthorSize = strlen(mapAuthor);
	char *mapVersion = "1.0";
	size_t mapVersionSize = strlen(mapVersion);
	char *mapDescription = "Test Description";
	size_t mapDescriptionSize = strlen(mapDescription);
	fwrite(&mapNameSize, sizeof(mapNameSize), 1, fp);
	fwrite(mapName, sizeof(*mapName) * mapNameSize, 1, fp);
	fwrite(&mapAuthorSize, sizeof(mapAuthorSize), 1, fp);
	fwrite(mapAuthor, sizeof(*mapAuthor) * mapAuthorSize, 1, fp);
	fwrite(&mapVersionSize, sizeof(mapVersionSize), 1, fp);
	fwrite(mapVersion, sizeof(*mapVersion) * mapVersionSize, 1, fp);
	fwrite(&mapDescriptionSize, sizeof(mapDescriptionSize), 1, fp);
	fwrite(mapDescription, sizeof(*mapDescription) * mapDescriptionSize, 1, fp);
	// Write the gravity
	float gravity = tsee->world->gravity;
	fwrite(&gravity, sizeof(gravity), 1, fp);
	// Write the number of textures
	size_t numTextures = tsee->textures->size;
	fwrite(&numTextures, sizeof(numTextures), 1, fp);
	for (size_t i = 0; i < tsee->textures->size; i++) {
		TSEE_Texture *texture = TSEEArrayGet(tsee->textures, i);
		size_t pathSize = strlen(texture->path);
		char toWrite[pathSize];
		memset(toWrite, 0, sizeof(char) * pathSize);
		for (size_t j = 0; j < pathSize; j++) {
			toWrite[j] = texture->path[j];
		}
		fwrite(&pathSize, sizeof(pathSize), 1, fp);
		fwrite(texture->path, pathSize * sizeof(char), 1, fp);
	}
	// Write the parallax backgrounds with texture indexes.
	fwrite(&tsee->world->parallax->size, sizeof(tsee->world->parallax->size), 1, fp);
	for (size_t i = 0; i < tsee->world->parallax->size; i++) {
		TSEE_Parallax *parallax = TSEEArrayGet(tsee->world->parallax, i);
		for (size_t j = 0; j < tsee->textures->size; j++) {
			TSEE_Texture *texture = TSEEArrayGet(tsee->textures, j);
			if (texture == parallax->texture) {
				uint64_t textureIdx = j;
				fwrite(&textureIdx, sizeof(textureIdx), 1, fp);
				break;
			}
		}
		fwrite(&parallax->distance, sizeof(parallax->distance), 1, fp);
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