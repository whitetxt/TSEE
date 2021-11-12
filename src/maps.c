#include "include/main.h"

size_t mapSize = 0;

struct Tile *map = NULL;
struct Map m;

size_t interactableMapSize = 0;
struct Interactable *interactableMap = NULL;

// Loads a map from a file
// @param path The path to the map file.
// @return 0 on success, 1 on failure.
int loadMap(char *path) {
	// Function to load a map from a file.

	FILE *fp = fopen(path, "rb");

	if (!fp) {
		printf("Unable to open file to read map.\n");
		return 1;
	}

	fread(&m.MapName, sizeof(m.MapName), 1, fp);
	printf("Loading into '%s'\n", m.MapName);
	fread(&m.maxScrollX, sizeof(m.maxScrollX), 1, fp);
	fread(&m.nTexturePaths, sizeof(m.nTexturePaths), 1, fp);
	fread(&m.nInteractablePaths, sizeof(m.nInteractablePaths), 1, fp);
	fread(&m.nTiles, sizeof(m.nTiles), 1, fp);
	fread(&m.nInteractables, sizeof(m.nInteractables), 1, fp);
	char buf[2000];
	int BUFSIZE = 2000;
	for (size_t i = 0; i < m.nTexturePaths; i++) {
		fgets(buf, BUFSIZE, fp);
		buf[strcspn(buf, "\r\n")] = 0;
		SDL_Texture *tmpTexture = IMG_LoadTexture(renderer, buf);
		if (!tmpTexture) {
			printf("Failed to load image %s.\n", buf);
			// Create black box to replace image if it could not be loaded.
			SDL_Surface *tmpsurf = SDL_CreateRGBSurface(0, 70, 70, 32, 0, 0, 0, 0);
			tmpTexture = SDL_CreateTextureFromSurface(renderer, tmpsurf);
			SDL_FreeSurface(tmpsurf);
		}
		loadedTextures = realloc(loadedTextures, ++textureSize * sizeof(*loadedTextures));
		loadedTextures[textureSize - 1] = tmpTexture;
	}

	for (size_t i = 0; i < m.nInteractablePaths; i++) {
		fgets(buf, BUFSIZE, fp);
		buf[strcspn(buf, "\r\n")] = 0;
		SDL_Texture *tmpTexture = IMG_LoadTexture(renderer, buf);
		if (!tmpTexture) {
			printf("Failed to load image %s.\n", buf);
			// Create black box to replace image if it could not be loaded.
			SDL_Surface *tmpsurf = SDL_CreateRGBSurface(0, 70, 70, 32, 0, 0, 0, 0);
			tmpTexture = SDL_CreateTextureFromSurface(renderer, tmpsurf);
			SDL_FreeSurface(tmpsurf);
		}
		loadedInteractablesInactive = realloc(loadedInteractablesInactive, ++interactableSize * sizeof(*loadedInteractablesInactive));
		loadedInteractablesInactive[interactableSize - 1] = tmpTexture;
	}

	loadedInteractablesActive = realloc(loadedInteractablesActive, interactableSize * sizeof*(loadedInteractablesActive));
	for (size_t i = 0; i < m.nInteractablePaths; i++) {
		fgets(buf, BUFSIZE, fp);
		buf[strcspn(buf, "\r\n")] = 0;
		SDL_Texture *tmpTexture = IMG_LoadTexture(renderer, buf);
		if (!tmpTexture) {
			printf("Failed to load image %s.\n", buf);
			// Create black box to replace image if it could not be loaded.
			SDL_Surface *tmpsurf = SDL_CreateRGBSurface(0, 70, 70, 32, 0, 0, 0, 0);
			tmpTexture = SDL_CreateTextureFromSurface(renderer, tmpsurf);
			SDL_FreeSurface(tmpsurf);
		}
		loadedInteractablesActive[interactableSize - 1] = tmpTexture;
	}
	m.Tiles = malloc(m.nTiles * sizeof(*m.Tiles));
	for (size_t i = 0; i < m.nTiles; i++) {
		fread(&m.Tiles[i].texIndex, sizeof(m.Tiles[i].texIndex), 1, fp);
		fread(&m.Tiles[i].x, sizeof(m.Tiles[i].x), 1, fp);
		fread(&m.Tiles[i].y, sizeof(m.Tiles[i].y), 1, fp);
		map = realloc(map, ++mapSize * sizeof(*map));
		map[mapSize - 1] = createTileFromTexture(loadedTextures[m.Tiles[i].texIndex], m.Tiles[i].x, m.Tiles[i].y);
	}
	free(m.Tiles);
	m.Interactables = malloc(m.nInteractables * sizeof(*m.Interactables));
	for (size_t i = 0; i < m.nInteractables; i++) {
		fread(&m.Interactables[i].texIndex, sizeof(m.Interactables[i].texIndex), 1, fp);
		fread(&m.Interactables[i].x, sizeof(m.Interactables[i].x), 1, fp);
		fread(&m.Interactables[i].y, sizeof(m.Interactables[i].y), 1, fp);
		fread(&m.Interactables[i].active, sizeof(m.Interactables[i].active), 1, fp);
		interactableMap = realloc(interactableMap, ++interactableMapSize * sizeof(*interactableMap));
		interactableMap[interactableMapSize - 1] = createInteractableFromTextures(m.Interactables[i].texIndex, m.Interactables[i].x, m.Interactables[i].y, m.Interactables[i].active);
	}
	free(m.Interactables);

	printf("'%s' loaded.\n", m.MapName);
	// Close the file
	fclose(fp);
	return 0;
}