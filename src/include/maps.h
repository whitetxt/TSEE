typedef struct Map {
	uint8_t MapName[32];
	int64_t maxScrollX;
	uint64_t nTexturePaths;
	uint64_t nInteractablePaths;
	uint64_t nTiles;
	uint64_t nInteractables;
	char **TexturePaths;
	char **InteractableActivePaths;
	char **InteractableInactivePaths;
	struct TileSerialisation *Tiles;
	struct InteractableSerialisation *Interactables;
} Map;

int loadMap(char *path);
void loadTextures(char *path);

extern size_t mapSize;
extern struct Tile *map;
extern struct Map m;

extern size_t interactableMapSize;
extern struct Interactable *interactableMap;