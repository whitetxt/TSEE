typedef struct Player {
	SDL_Texture *tex;
	SDL_Texture *crouchTex;
	SDL_Rect rect;
	SDL_Rect worldRect;
	bool onGround;
	bool isCrouching;
	float normSpeed;
	float crouchSpeed;
	int dir;
} Player;

extern uint16_t worldScrollX;

struct Player createPlayer(char *fp, char *crouchfp, int x, int y, float normSpeed, float crouchSpeed);

void movePlayer(Player *Player, int dir);

void grav(Player *Player);

void playerJump(Player *Player);

bool checkCollision(SDL_Rect Rect);

void playerCollision(Player *Player);
void updateWorldScroll(Player *Player);

void startCrouch(Player *Player);
void endCrouch(Player *Player);

void renderPlayer(Player *Player);