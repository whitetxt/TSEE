#include "include/main.h"

// Creates an SDL_Rect from a texture and a position.
// @param tex The texture to create the rect for.
// @param x The x position of the rect.
// @param y The y position of the rect.
// @return A SDL_Rect
SDL_Rect createRect(SDL_Texture *tex, int x, int y) {
	// Function to create a SDL_Rect from a texture and position.
	SDL_Rect rect;
	SDL_QueryTexture(tex, NULL, NULL, &rect.w, &rect.h);
	rect.x = x;
	rect.y = y;
	return rect;
}

// Loads a texture from a file.
// @param renderer The renderer to load the texture to.
// @param fp The file path to load the texture from.
// @return The new SDL_Texture.
SDL_Texture *loadImage(SDL_Renderer *renderer, char *fp) {
	// Function to create a texture from an image location.
	SDL_Texture *tmptex = IMG_LoadTexture(renderer, fp);
	// Check if the image was loaded correctly
	if (!tmptex) {
		printf("Failed to load image %s.\n", fp);
		// Create black box to replace image if it could not be loaded.
		SDL_Surface *tmpsurf = SDL_CreateRGBSurface(0, 50, 50, 32, 0, 0, 0, 0);
		tmptex = SDL_CreateTextureFromSurface(renderer, tmpsurf);
		SDL_FreeSurface(tmpsurf);
	}
	return tmptex;
}

// Function to create a struct Tile from a SDL_Texture and SDL_Rect.
// @param tex The texture to create the tile from.
// @param rect The SDL_Rect to create the tile from.
// @return The new Tile.
struct Tile almostCreateTile(SDL_Texture *tex, SDL_Rect rect) {
	struct Tile tmpTile;
	tmpTile.tex = tex;
	tmpTile.rect = rect;
	tmpTile.worldRect = rect;
	return tmpTile;
}

// Creates a tile from a file path and position.
// @param fp The file path to load the tile from.
// @param x The x position of the tile.
// @param y The y position of the tile.
// @return The new Tile.
struct Tile createTile(char *fp, int x, int y) {
	SDL_Texture *tmpTex = loadImage(renderer, fp);
	SDL_Rect tmpRect = createRect(tmpTex, x, y);
	struct Tile tmpTile = almostCreateTile(tmpTex, tmpRect);
	return tmpTile;
}

// Creates a tile from a texture
// @param tmpTex The texture to create the tile from.
// @param x The x position of the tile.
// @param y The y position of the tile.
// @return The new Tile.
struct Tile createTileFromTexture(SDL_Texture *tmpTex, int x, int y) {
	// Function to create a Tile from an already loaded texture.
	SDL_Rect tmpRect = createRect(tmpTex, x, y);
	struct Tile tmpTile = almostCreateTile(tmpTex, tmpRect);
	return tmpTile;
}

// Creates an interactable from an index into the interactables array.
// @param index The index of the interactable to create.
// @param x The x position of the interactable.
// @param y The y position of the interactable.
// @param active Whether the interactable is active or not.
// @return The new interactable.
struct Interactable createInteractableFromTextures(int texIndex, int x, int y, uint8_t active) {
	SDL_Rect activeRect = createRect(loadedInteractablesActive[texIndex], x, y);
	SDL_Rect inactiveRect = createRect(loadedInteractablesInactive[texIndex], x, y);
	struct Interactable toReturn;
	toReturn.activeTex = loadedInteractablesActive[texIndex];
	toReturn.inactiveTex = loadedInteractablesInactive[texIndex];
	if (active == 1) {
		toReturn.rect = activeRect;
		toReturn.active = true;
	} else {
		toReturn.rect = inactiveRect;
		toReturn.active = false;
	}
	toReturn.worldRect = toReturn.rect;
	return toReturn;
}

// Collision function for interactables.
// @param Player the player to check for collision with.
void interactables(Player *Player) {
	SDL_Rect Rect = Player->rect;
	for (size_t x = 0; x < interactableMapSize; x++) {

		// If the tile is too far away, dont waste time checking.
		if (abs(Rect.x - interactableMap[x].rect.x) > 100)
			continue;
		if (abs(Rect.y - interactableMap[x].rect.y) > 100)
			continue;

		// First, check if there is a collision.
		if(	Rect.x + Rect.w > interactableMap[x].rect.x &&
			Rect.x < interactableMap[x].rect.x + interactableMap[x].rect.w &&
			Rect.y + Rect.h > interactableMap[x].rect.y &&
			Rect.y < interactableMap[x].rect.y + interactableMap[x].rect.h) {
			interactableMap[x].active = !interactableMap[x].active;
		}
	}
}