typedef struct Tile {
	SDL_Texture *tex;
	SDL_Rect rect;
	SDL_Rect worldRect;
} Tile;

typedef struct TileSerialisation {
	uint64_t texIndex;
	uint32_t x, y;
} TileSerialisation;

typedef struct Interactable {
	SDL_Texture *activeTex;
	SDL_Texture *inactiveTex;
	SDL_Rect rect;
	SDL_Rect worldRect;
	bool active;
} Interactable;

typedef struct InteractableSerialisation {
	uint64_t texIndex;
	uint32_t x, y;
	uint8_t active;
} InteractableSerialisation;

typedef struct OtherPlayer {
	uint16_t ID;
	char *name;
	bool crouching;
	SDL_Rect rect;
	SDL_Rect worldRect;
	Text nameTag;
} OtherPlayer;

SDL_Rect createRect(SDL_Texture *tex, int x, int y);

SDL_Texture *loadImage(SDL_Renderer *renderer, char *fp);

struct Tile almostCreateTile(SDL_Texture *tex, SDL_Rect rect);

struct Tile createTile(char *fp, int x, int y);

struct Tile createTileFromTexture(SDL_Texture *tmpTex, int x, int y);

struct Interactable createInteractableFromTextures(int texIndex, int x, int y, uint8_t active);

void interactables(Player *Player);