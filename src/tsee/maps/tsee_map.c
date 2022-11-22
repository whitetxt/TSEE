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
 * @brief Writes a null-terminated string to a file.
 *
 * @param fp File to write to
 * @param string String to write
 */
void TSEE_WriteFile_String(FILE *fp, char *string) {
	TSEE_WriteFile(string, sizeof(*string) * strlen(string), 1, fp);
	putc(0, fp);
}

/**
 * @brief Reads from a file
 *
 * @param dst Destination buffer
 * @param size Size to read
 * @param n Number of items to read
 * @param fp File to read from
 * @return success status
 */
bool TSEE_ReadFile(void *dst, size_t size, size_t n, FILE *fp) {
	if (fread(dst, size, n, fp) != n) {
		TSEE_Error("Failed to read from file (or reached unexpected EOF)\n");
		return false;
	}
	return true;
}

/**
 * @brief Writes to a file
 *
 * @param src Source buffer
 * @param size Size to write
 * @param n Number to items to write
 * @param fp File to write to
 * @return success status
 */
bool TSEE_WriteFile(void *src, size_t size, size_t n, FILE *fp) {
	if (fwrite(src, size, n, fp) != n) {
		TSEE_Error("Failed to write to file\n");
		return false;
	}
	return true;
}

/**
 * @brief Loads a TSEE map from a file
 *
 * @param fn File name to load from.
 * @return success status
 */
bool TSEE_Map_Load(TSEE *tsee, char *fn) {
	// Open the file
	FILE *fp = fopen(fn, "rb");
	if (!fp) {
		TSEE_Error("Failed to open map file (%s)\n", fn);
		return false;
	}

	// Clear arrays to be overwritten
	if (tsee->world->objects) {
		if (tsee->world->objects->data) {
			for (size_t i = 0; i < tsee->world->objects->size; i++) {
				TSEE_Object *obj = TSEE_Array_Get(tsee->world->objects, i);
				TSEE_Object_Destroy(tsee, obj, false);
			}
		}
	}
	TSEE_Resource_Unload(tsee);

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
	if (!(mapName = TSEE_ReadFile_UntilNull(fp)))
		return false;
	char *mapAuthor = NULL;
	if (!(mapAuthor = TSEE_ReadFile_UntilNull(fp)))
		return false;
	char *mapVersion = NULL;
	if (!(mapVersion = TSEE_ReadFile_UntilNull(fp)))
		return false;
	char *mapDescription = NULL;
	if (!(mapDescription = TSEE_ReadFile_UntilNull(fp)))
		return false;
	TSEE_Log("Loading into %s by %s\nVersion: %s\n%s\n", mapName, mapAuthor,
			 mapVersion, mapDescription);

	// Read the gravity (for some reason its here in the header?)
	float gravityX = 0;
	if (TSEE_ReadFile(&gravityX, sizeof(gravityX), 1, fp) != 1) {
		TSEE_Error("Failed to read gravity X.\n");
		return false;
	}
	float gravityY = 0;
	if (TSEE_ReadFile(&gravityY, sizeof(gravityY), 1, fp) != 1) {
		TSEE_Error("Failed to read gravity Y.\n");
		return false;
	}
	TSEE_World_SetGravity(tsee, (TSEE_Vec2){gravityX, gravityY});

	// Read map's textures
	size_t numTextures = 0;
	if (TSEE_ReadFile(&numTextures, sizeof(numTextures), 1, fp) != 1) {
		TSEE_Error("Failed to read number of textures.\n");
		return false;
	}
	TSEE_Log("Loading %zu textures\n", numTextures);
	char **texturePaths = NULL;
	size_t numTexPaths = 0;
	char *texturePath = NULL;
	for (size_t i = 0; i < numTextures; i++) {
		texturePath = NULL;
		if (!(texturePath = TSEE_ReadFile_UntilNull(fp)))
			return false;
		texturePaths =
			xrealloc(texturePaths, sizeof(*texturePaths) * (++numTexPaths));
		texturePaths[numTexPaths - 1] = strdup(texturePath);
		free(texturePath);
	}

	// Read objects
	size_t numObjects = 0;
	if (TSEE_ReadFile(&numObjects, sizeof(numObjects), 1, fp) != 1) {
		TSEE_Error("Failed to read number of objects.\n");
		return false;
	}
	TSEE_Log("Loading %zu objects\n", numObjects);
	for (size_t i = 0; i < numObjects; i++) {
		size_t texIdx = 0;
		if (TSEE_ReadFile(&texIdx, sizeof(texIdx), 1, fp) != 1) {
			TSEE_Error("Failed to read texture index.\n");
			return false;
		}
		TSEE_Log("Read object %ld with texture %ld\n", i, texIdx);
		char *path = texturePaths[texIdx];
		float x = 0;
		float y = 0;
		TSEE_Object_Attributes attr;

		if (TSEE_ReadFile(&x, sizeof(x), 1, fp) != 1 ||
			TSEE_ReadFile(&y, sizeof(y), 1, fp) != 1) {
			TSEE_Error("Failed to read object position\n");
			return false;
		}

		if (TSEE_ReadFile(&attr, sizeof(attr), 1, fp) != 1) {
			TSEE_Error("Failed to read object attributes\n");
			return false;
		}

		TSEE_Object *object;

		if (TSEE_Attributes_Check(attr, TSEE_ATTRIB_PARALLAX)) {
			object = TSEE_Parallax_Create(tsee, TSEE_Texture_Create(tsee, path),
										  1000);
		} else {
			object = TSEE_Object_Create(tsee, TSEE_Texture_Create(tsee, path),
										attr, x, y);
		}

		if (TSEE_Attributes_Check(attr, TSEE_ATTRIB_PHYS)) {
			TSEE_ReadFile(&object->physics.mass, sizeof(object->physics.mass),
						  1, fp);
			TSEE_ReadFile(&object->physics.restitution,
						  sizeof(object->physics.restitution), 1, fp);
		} else if (TSEE_Attributes_Check(attr, TSEE_ATTRIB_PARALLAX)) {
			TSEE_ReadFile(&object->parallax.distance,
						  sizeof(object->parallax.distance), 1, fp);
		} else if (TSEE_Attributes_Check(attr, TSEE_ATTRIB_TEXT)) {
			object->text.text = TSEE_ReadFile_UntilNull(fp);
		}

		TSEE_Log(
			"Loaded object `%s` at (%f, %f, %d, %d) with texture at (%d, %d)\n",
			object->texture->path, object->position.x, object->position.y,
			object->texture->rect.w, object->texture->rect.h,
			object->texture->rect.x, object->texture->rect.y);
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
	if (TSEE_ReadFile(&speed, sizeof(speed), 1, fp) != 1) {
		TSEE_Error("Failed to read player speed.\n");
		return false;
	}
	TSEE_Player_SetSpeed(tsee, speed);
	float jumpForce = 0;
	if (TSEE_ReadFile(&jumpForce, sizeof(jumpForce), 1, fp) != 1) {
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
 * @param fn File name to save to.
 * @return success status
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
	TSEE_WriteFile_String(fp, mapName);
	TSEE_WriteFile_String(fp, mapAuthor);
	TSEE_WriteFile_String(fp, mapVersion);
	TSEE_WriteFile_String(fp, mapDescription);
	// Write the gravity
	float gravityX = tsee->world->gravity.x;
	TSEE_WriteFile(&gravityX, sizeof(gravityX), 1, fp);
	float gravityY = tsee->world->gravity.y;
	TSEE_WriteFile(&gravityY, sizeof(gravityY), 1, fp);
	// Write the number of textures

	// As not all textures are loaded from files, we must figure out how many
	// unique ones there are.
	size_t numTextures = 0;
	char **texturesSeen = NULL;
	for (size_t i = 0; i < tsee->resources->textures->size; i++) {
		TSEE_Texture *texture = TSEE_Array_Get(tsee->resources->textures, i);
		if (!texture->path)
			continue;
		if (!texturesSeen) {
			texturesSeen = xmalloc(sizeof(*texturesSeen) * ++numTextures);
			if (!texturesSeen) {
				TSEE_Error("Failed to malloc for new texture.\n");
				return false;
			}
			texturesSeen[0] = strdup(texture->path);
		}
		bool found = false;
		for (size_t j = 0; j < numTextures; j++) {
			if (strcmp(texturesSeen[j], texture->path) == 0) {
				found = true;
				break;
			}
		}
		if (found)
			continue;
		texturesSeen =
			xrealloc(texturesSeen, sizeof(*texturesSeen) * (++numTextures));
		texturesSeen[numTextures - 1] = strdup(texture->path);
	}
	TSEE_Log("Found %ld unique textures.\n", numTextures);
	TSEE_WriteFile(&numTextures, sizeof(numTextures), 1, fp);

	// Then we write the paths for the textures.
	for (size_t i = 0; i < numTextures; i++) {
		TSEE_WriteFile_String(fp, texturesSeen[i]);
	}

	// Write the objects with texture indexes.
	TSEE_Log("Writing %ld objects.\n", tsee->world->objects->size);
	TSEE_WriteFile(&tsee->world->objects->size,
				   sizeof(tsee->world->objects->size), 1, fp);
	for (size_t i = 0; i < tsee->world->objects->size; i++) {
		bool written = false;
		TSEE_Object *object = TSEE_Array_Get(tsee->world->objects, i);
		for (size_t j = 0; j < numTextures; j++) {
			if (strcmp(object->texture->path, texturesSeen[j]) == 0) {
				TSEE_Log("Written object %ld with texture %ld.\n", i, j);
				TSEE_WriteFile(&j, sizeof(j), 1, fp);
				written = true;
				break;
			}
		}
		if (!written) {
			TSEE_Error("Couldn't write texture idx for texture!!\n");
			return false;
		}
		TSEE_WriteFile(&object->position.x, sizeof(object->position.x), 1, fp);
		TSEE_WriteFile(&object->position.y, sizeof(object->position.y), 1, fp);
		TSEE_WriteFile(&object->attributes, sizeof(object->attributes), 1, fp);
		// Write individual fields for each attribute
		if (TSEE_Object_CheckAttribute(object, TSEE_ATTRIB_PHYS)) {
			TSEE_WriteFile(&object->physics.mass, sizeof(object->physics.mass),
						   1, fp);
			TSEE_WriteFile(&object->physics.restitution,
						   sizeof(object->physics.restitution), 1, fp);
		} else if (TSEE_Object_CheckAttribute(object, TSEE_ATTRIB_PARALLAX)) {
			TSEE_WriteFile(&object->parallax.distance,
						   sizeof(object->parallax.distance), 1, fp);
		} else if (TSEE_Object_CheckAttribute(object, TSEE_ATTRIB_TEXT)) {
			TSEE_WriteFile_String(fp, object->text.text);
		}
	}

	// Write player information
	TSEE_WriteFile(&tsee->player->speed, sizeof(tsee->player->speed), 1, fp);
	TSEE_WriteFile(&tsee->player->jump_force, sizeof(tsee->player->jump_force),
				   1, fp);
	fclose(fp);
	TSEE_Log("Saved map to %s\n", fn);
	return true;
}