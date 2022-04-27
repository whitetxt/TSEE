#include "../tsee.h"

/**
 * @brief Reads from a file pointer until a null character is hit.
 * 
 * @param fp File pointer to read from.
 * @return char* 
 */
char *TSEE_ReadFile_UntilNull(FILE *fp) {
	char c;
	int len = 0;
	char *buffer = NULL;
	while ((c = getc(fp)) != 0 && c != EOF) {
		char *newPtr = xrealloc(buffer, sizeof(*buffer) * (++len));
		if (!newPtr) {
			TSEE_Critical("Failed realloc for text buffer.\n");
			return NULL;
		}
		buffer = newPtr;
		buffer[len - 1] = c;
	}
	buffer[len] = '\0';
	return buffer;
}

/**
 * @brief Loads a TSEE map from a file
 * 
 * @param tsee TSEE object to load the map into.
 * @param fn File name to load from.
 * @return true on success, false on fail.
 */
bool TSEE_Map_Load(TSEE *tsee, char *fn) {
	// Open the file
	FILE *fp = fopen(fn, "rb");
	if (fp == NULL) {
		TSEE_Error("Failed to open map file (%s)\n", fn);
		return false;
	}

	// Clear arrays to be overwritten
	TSEE_Array_Clear(tsee->world->objects);

	// Reset the player
	tsee->player->grounded = true;
	tsee->player->jump_force = 0;
	tsee->player->movement.down = false;
	tsee->player->movement.up = false;
	tsee->player->movement.left = false;
	tsee->player->movement.right = false;
	tsee->player->object = NULL;
	tsee->player->speed = 0;

	// Read all of the map header data
	char *mapName = NULL;
	if (!(mapName = TSEE_ReadFile_UntilNull(fp))) return false;
	char *mapAuthor = NULL;
	if (!(mapAuthor = TSEE_ReadFile_UntilNull(fp))) return false;
	char *mapVersion = NULL;
	if (!(mapVersion = TSEE_ReadFile_UntilNull(fp))) return false;
	char *mapDescription = NULL;
	if (!(mapDescription = TSEE_ReadFile_UntilNull(fp))) return false;
	TSEE_Log("Loading into %s by %s\nVersion: %s\n%s\n", mapName, mapAuthor, mapVersion, mapDescription);

	// Read the gravity (for some reason its here in the header?)
	float gravityX = 0;
	if (fread(&gravityX, sizeof(gravityX), 1, fp) != 1) {
		TSEE_Error("Failed to read gravity X.\n");
		return false;
	}
	float gravityY = 0;
	if (fread(&gravityY, sizeof(gravityY), 1, fp) != 1) {
		TSEE_Error("Failed to read gravity Y.\n");
		return false;
	}
	TSEE_World_SetGravity(tsee, (TSEE_Vec2){gravityX, gravityY});

	// Read map's textures
	size_t numTextures = 0;
	if (fread(&numTextures, sizeof(numTextures), 1, fp) != 1) {
		TSEE_Error("Failed to read number of textures.\n");
		return false;
	}
	TSEE_Log("Loading %zu textures\n", numTextures);
	for (size_t i = 0; i < numTextures; i++) {
		char *texturePath = NULL;
		if (!(texturePath = TSEE_ReadFile_UntilNull(fp))) return false;
		TSEE_Texture *texture = TSEE_Texture_Create(tsee, texturePath);
		if (!texture) {
			TSEE_Error("Failed to load texture `%s` (%s)\n", texturePath, SDL_GetError());
			return false;
		}
	}

	// Read objects
	size_t numObjects = 0;
	if (fread(&numObjects, sizeof(numObjects), 1, fp) != 1) {
		TSEE_Error("Failed to read number of objects.\n");
		return false;
	}
	TSEE_Log("Loading %zu objects\n", numObjects);
	for (size_t i = 0; i < numObjects; i++) {
		size_t texIdx = 0;
		if (fread(&texIdx, sizeof(texIdx), 1, fp) != 1) {
			TSEE_Error("Failed to read texture index.\n");
			return false;
		}
		TSEE_Texture *tex = TSEE_Array_Get(tsee->textures, texIdx);
		TSEE_Object *object = TSEE_Object_Create(tsee, TSEE_Texture_Create(tsee, tex->path), TSEE_ATTRIB_NONE, 0, 0);
		if (fread(&object->position.x, sizeof(object->position.x), 1, fp) != 1) {
			TSEE_Error("Failed to read object position\n");
			return false;
		}
		if (fread(&object->position.y, sizeof(object->position.y), 1, fp) != 1) {
			TSEE_Error("Failed to read object position\n");
			return false;
		}
		object->texture->rect.x = object->position.x;
		object->texture->rect.y = object->position.y;
		if (fread(&object->attributes, sizeof(object->attributes), 1, fp) != 1) {
			TSEE_Error("Failed to read object attributes\n");
			return false;
		}
		TSEE_Log("Loaded object `%s` at (%f, %f, %d, %d) with texture at (%d, %d)\n", object->texture->path, object->position.x, object->position.y, object->texture->rect.w, object->texture->rect.h, object->texture->rect.x, object->texture->rect.y);
	}

	// Setup the player
	bool player_found = false;
	for (size_t i = 0; i < tsee->world->objects->size; i++) {
		TSEE_Object *obj = TSEE_Array_Get(tsee->world->objects, i);
		if (TSEE_Object_CheckAttribute(obj, TSEE_ATTRIB_PLAYER)) {
			tsee->player->object = obj;
			player_found = true;
			break;
		}
	}
	if (!player_found) {
		TSEE_Warn("Failed to find player object.\n");
	}

	float speed = 0;
	if (fread(&speed, sizeof(speed), 1, fp) != 1) {
		TSEE_Error("Failed to read player speed.\n");
		return false;
	}
	TSEE_Player_SetSpeed(tsee, speed);
	float jumpForce = 0;
	if (fread(&jumpForce, sizeof(jumpForce), 1, fp) != 1) {
		TSEE_Error("Failed to read player jump force.\n");
		return false;
	}
	TSEE_Player_SetJumpForce(tsee, jumpForce);

	fclose(fp);
	TSEE_Log("Map %s loaded successfully.\n", mapName);
	return true;
}

/**
 * @brief Saves the current TSEE to a map file
 * 
 * @param tsee TSEE object to save.
 * @param fn File name to save to.
 * @return true on success, false on fail.
 */
bool TSEE_Map_Save(TSEE *tsee, char *fn) {
	FILE *fp = fopen(fn, "wb");
	if (fp == NULL) {
		TSEE_Error("Failed to open map file (%s)\n", fn);
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
	float gravityX = tsee->world->gravity.x;
	fwrite(&gravityX, sizeof(gravityX), 1, fp);
	float gravityY = tsee->world->gravity.y;
	fwrite(&gravityY, sizeof(gravityY), 1, fp);
	// Write the number of textures
	size_t numTextures = tsee->textures->size;
	fwrite(&numTextures, sizeof(numTextures), 1, fp);
	for (size_t i = 0; i < tsee->textures->size; i++) {
		TSEE_Texture *texture = TSEE_Array_Get(tsee->textures, i);
		int pathSize = strlen(texture->path);
		fwrite(texture->path, pathSize * sizeof(char), 1, fp);
		putc(0, fp);
	}
	// Write the objects with texture indexes.
	fwrite(&tsee->world->objects->size, sizeof(tsee->world->objects->size), 1, fp);
	for (size_t i = 0; i < tsee->world->objects->size; i++) {
		bool written = false;
		TSEE_Object *object = TSEE_Array_Get(tsee->world->objects, i);
		for (size_t j = 0; j < tsee->textures->size; j++) {
			TSEE_Texture *texture = TSEE_Array_Get(tsee->textures, j);
			if (strcmp(texture->path, object->texture->path) == 0) {
				size_t textureIdx = j;
				fwrite(&textureIdx, sizeof(textureIdx), 1, fp);
				written = true;
				break;
			}
		}
		if (!written) {
			TSEE_Error("Couldn't write texture idx for texture!!\n");
		}
		fwrite(&object->position.x, sizeof(object->position.x), 1, fp);
		fwrite(&object->position.y, sizeof(object->position.y), 1, fp);
		fwrite(&object->attributes, sizeof(object->attributes), 1, fp);
	}

	// Write player information
	fwrite(&tsee->player->speed, sizeof(tsee->player->speed), 1, fp);
	fwrite(&tsee->player->jump_force, sizeof(tsee->player->jump_force), 1, fp);
	fclose(fp);
	TSEE_Log("Saved map to %s\n", fn);
	return true;
}