#include "include/main.h"

double PrevGrav = 0;
int BaseGrav = 0;
uint16_t worldScrollX = 0;
double PlayerAccel = 0;

// Creates a player object
// @param fp The path to the image to use for the player
// @param crouchfp The path to the image to use for the player when crouching
// @param x The x position of the player
// @param y The y position of the player
// @param normSpeed The normal speed of the player
// @param crouchSpeed The speed of the player when crouching
// @return The newly created player object
Player createPlayer(char *fp, char *crouchfp, int x, int y, float normSpeed, float crouchSpeed) {
	Player tmpPlayer;
	tmpPlayer.tex = loadImage(renderer, fp);
	tmpPlayer.crouchTex = loadImage(renderer, crouchfp);
	tmpPlayer.rect = createRect(tmpPlayer.tex, x, y);
	tmpPlayer.worldRect = tmpPlayer.rect;
	tmpPlayer.normSpeed = normSpeed;
	tmpPlayer.crouchSpeed = crouchSpeed;
	return tmpPlayer;
}

// Handles player movement
// @param Player The player to move
// @param dir The direction to move in (1 = E, 3 = W)
void movePlayer(Player *Player, int dir) {
	// Function to move the player around. Also handles scrolling.
	/* 	0 = N
	1 = E
	2 = S
	3 = W */
	// Change player speed if crouching.
	float speed;
	if (Player->isCrouching)
		speed = Player->crouchSpeed;
	else 
		speed = Player->normSpeed;
	switch (dir) {
		case 1:
			PlayerAccel += speed;
			break;
		case 3:
			PlayerAccel -= speed;
			break;
	}
}

// Starts crouch mode.
// @param Player: The player to start crouch mode on.
void startCrouch(Player *Player) {
	Player->rect.h = 35;
	Player->isCrouching = true;
	Player->rect.y += 35;
	if (onServer)
		crouchPacket(Player->isCrouching);
}

// Ends crouch mode.
// @param Player: The player to end crouch mode on.
void endCrouch(Player *Player) {
	Player->rect.h = 70;
	Player->rect.y -= 35;
	// Checks if player would get clipped inside a block if we uncrouched.
	// First check will push the player downwards if they collide with the block above when uncrouched.
	if (checkCollision(Player->rect)) {
		// Second check will push the player upwards if they collide with the block below.
		if (checkCollision(Player->rect)) {
			// If we collide above then below, we cannot uncrouch.
			Player->rect.h = 35;
			Player->rect.y += 35;
			return;
		}
	}
	Player->isCrouching = false;
	if (onServer)
		crouchPacket(Player->isCrouching);
}

// Applies gravity to a player.
// @param Player The player to apply gravity to.
void grav(Player *Player) {
	// Function to apply accelerating gravity.
	PrevGrav += 12 * dt;
	Player->rect.y += PrevGrav;
	PlayerAccel = PlayerAccel / 1.25;
	if (PlayerAccel < 0.05 && PlayerAccel > -0.1)
		PlayerAccel = 0;
	Player->rect.x = Player->rect.x + PlayerAccel * dt;
	
}

// Updates world scroll
// @param Player The player to center for world scroll.
void updateWorldScroll(Player *Player) {
	// If the player is at the center and moving to the right:
	if (Player->rect.x + (Player->rect.w / 2) > width / 2 && m.maxScrollX > worldScrollX + width) {
		// Change the world scroll.
		worldScrollX = worldScrollX + abs(width / 2 - (Player->rect.x + Player->rect.w/2));
		// Prevent worldScrollX from exceeding bounds.
		if (worldScrollX > m.maxScrollX)
			worldScrollX = m.maxScrollX;
		// Lock the player at the center.
		Player->rect.x = (width / 2) - (Player->rect.w / 2);
		// Change the positions of all the tiles.
		for (size_t x = 0; x < mapSize; x++) {
			map[x].rect.x = map[x].worldRect.x - worldScrollX;
		}
		for (size_t x = 0; x < numPlayers; x++) {
			players[x].rect.x = players[x].worldRect.x - worldScrollX;
		}
	}
	// If the player is at the center, not at the left edge and moving to the left:
	if (worldScrollX != 0 && Player->rect.x + (Player->rect.w / 2) < width / 2) {
		// Change the world scroll.
		if (abs(width / 2 - (Player->rect.x + Player->rect.w/2)) > worldScrollX) {
			worldScrollX = 0;
		} else {
			worldScrollX = worldScrollX - abs(width / 2 - (Player->rect.x + Player->rect.w/2));
		}
		// Lock the world scroll at the left edge.
		// Lock the player at the center.
		Player->rect.x = (width / 2) - (Player->rect.w / 2);
		// Update positions of all the tiles.
		for (size_t x = 0; x < mapSize; x++) {
			map[x].rect.x = map[x].worldRect.x - worldScrollX;
		}
		for (size_t x = 0; x < numPlayers; x++) {
			players[x].rect.x = players[x].worldRect.x - worldScrollX;
		}
	}
}

// Makes a player jump.
// @param Player The player to make jump.
void playerJump(Player *Player) {
	// Simple function that uses gravity to make the player jump.
	if (Player->onGround) {
		PrevGrav = -4;
		Player->onGround = false;
	}
}

// Handles collision for an SDL_Rect.
// @param Rect the SDL_Rect to handle collisions for.
// @return true if collision occurred, false otherwise.
bool checkCollision(SDL_Rect Rect) {
	for (size_t x = 0; x < mapSize; x++) {

		// If the tile is too far away, dont waste time checking.
		if (abs(Rect.x - map[x].rect.x) > 100)
			continue;
		if (abs(Rect.y - map[x].rect.y) > 100)
			continue;

		// First, check if there is a collision.
		if(	Rect.x + Rect.w > map[x].rect.x &&
			Rect.x < map[x].rect.x + map[x].rect.w &&
			Rect.y + Rect.h > map[x].rect.y &&
			Rect.y < map[x].rect.y + map[x].rect.h) {
			// Figure out collision side
			int amtRight = abs(Rect.x + Rect.w - map[x].rect.x);
			int amtLeft = abs(map[x].rect.x + map[x].rect.w - Rect.x);
			int amtTop = abs(map[x].rect.y - Rect.y + Rect.h);
			int amtBottom = abs(Rect.y + Rect.h - map[x].rect.y);

			int values[4] = {amtRight, amtLeft, amtTop, amtBottom};
			int lowest = values[0];
			// Get lowest value, side it collided on
			for (int x = 0; x < 4; x++) {
				if (values[x] < lowest) {
					lowest = values[x];
				}
			}

			// Handle the collision side appropriately.
			if (lowest == amtRight)
				Rect.x = map[x].rect.x - Rect.w;
			else if (lowest == amtLeft)
				Rect.x = map[x].rect.x + map[x].rect.w;
			else if (lowest == amtTop)
				Rect.y = map[x].rect.y + map[x].rect.h;
			else if (lowest == amtBottom)
				Rect.y = map[x].rect.y - Rect.h;
			return true;
		}
	}
	return false;
}

// Handles collision for a player.
// @param Player The player to handle collisions for.
void playerCollision(Player *Player) {
	// If the player is too low down, clamp at the bottom.
	if (Player->rect.y + Player->rect.h > height) {
		worldScrollX = 0;
		Player->rect.x = 0;
		Player->rect.y = 0;
		for (size_t x = 0; x < mapSize; x++) {
			map[x].rect.x = map[x].worldRect.x - worldScrollX;
		}
		PrevGrav = 0;
	}

	// If too far left, clamp at the left side.
	if (Player->rect.x < 0)
		Player->rect.x = 0;

	// If too far right, clamp at the right side.
	if (Player->rect.x + Player->rect.w > width)
		Player->rect.x = width - Player->rect.w;

	// If too high, clamp at the top.
	if (Player->rect.y < 0) {
		Player->rect.y = 0;
		PrevGrav = 0;
	}

	for (size_t x = 0; x < mapSize; x++) {

		// If the tile is too far away, don't waste time checking for collision.
		if (abs(Player->rect.x - map[x].rect.x) > 80)
			continue;
		if (abs(Player->rect.y - map[x].rect.y) > 80)
			continue;

		// First, check if it collides.
		if(	Player->rect.x + Player->rect.w > map[x].rect.x &&
			Player->rect.x < map[x].rect.x + map[x].rect.w &&
			Player->rect.y + Player->rect.h > map[x].rect.y &&
			Player->rect.y < map[x].rect.y + map[x].rect.h) {
			//printf("Collision\n");
			// Figure out collision side
			int amtRight = abs(Player->rect.x + Player->rect.w - map[x].rect.x);
			int amtLeft = abs(map[x].rect.x + map[x].rect.w - Player->rect.x);
			int amtTop = abs(map[x].rect.y - Player->rect.y + Player->rect.h);
			int amtBottom = abs(Player->rect.y + Player->rect.h - map[x].rect.y);

			int values[4] = {amtRight, amtLeft, amtTop, amtBottom};
			int lowest = values[0];
			// Get lowest value, side it collided on
			for (int x = 0; x < 4; x++) {
				if (values[x] < lowest) {
					lowest = values[x];
				}
			}

			// Handle collision side accordingly.
			if (lowest == amtRight) {
				Player->rect.x = map[x].rect.x - Player->rect.w;
				//printf("Collision Right\n");
			} else if (lowest == amtLeft) {
				Player->rect.x = map[x].rect.x + map[x].rect.w;
				//printf("Collision Left\n");
			} else if (lowest == amtTop) {
				Player->rect.y = map[x].rect.y + map[x].rect.h;
				if (PrevGrav < 0)
					PrevGrav = 0;
				//printf("Collision Top\n");
			} else if (lowest == amtBottom) {
				Player->rect.y = map[x].rect.y - Player->rect.h;
				Player->onGround = true;
				if (PrevGrav > 0)
					PrevGrav = 0;
				//printf("Collision Bottom\n");
			}
		}
	}
}

// Renders a player.
// @param Player The player to render.
void renderPlayer(Player *player) {
	if (player->isCrouching)
		SDL_RenderCopy(renderer, player->crouchTex, NULL, &player->rect);
	else
		SDL_RenderCopy(renderer, player->tex, NULL, &player->rect);
}