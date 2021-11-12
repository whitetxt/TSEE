#define ZED_NET_IMPLEMENTATION
#include <zed_net.h>
#include "include/main.h"

void render() {
	// Main render function. renders the player, all the tiles and all text
}

void calculateDT() {
	// Calculates delta time
}

void calculateFPS() {
	// Calculates the FPS from deltatime.
	if (dt != 0) {
		// Before I turned vsync on, dt was sometimes 0, therefore crashing when divide by 0.
		double EstFPS = 1000 / (dt * 1000);
		printf("FPS: %f - %fms taken.\n", EstFPS, dt * 1000);
	} else {
		printf("FPS: INF - 0ms taken.\n");
	}
	return;
}

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;
	printf("Started.\n");

	loadSettings();

	if (initWindow() != 0) {
		printf("Window or renderer didn't initialise properly");
		return 1;
	}

	initFonts();
	SDL_StartTextInput();

	// Create the player
	MainPlayer = createPlayer("textures/Player.png", "textures/PlayerCrouch.png", 0, 0, 100, 40);

	if (mainMenu() == -1) {
		playing = false;
	} else {
		playing = true;
	}
	createMenu("main.menu");
	paused = true;

	// Main event loop.
	while (playing) {
		if (!paused) {
			// eventHandling returns 1 when it wants to quit the game.
			if (eventHandling(&MainPlayer) == 1) {
				break;
			}
			// All of these require passing the player through to allow for potential multiplayer.
			grav(&MainPlayer);
			playerCollision(&MainPlayer);
			updateWorldScroll(&MainPlayer);
			if (onServer) { // If multiplayer
				movePacket();
				updateMultiplayerNames();
			}
			render();
		} else {
			int retVal = pauseEventHandling();
			if (retVal == 1) {
				break;
			} else if (retVal == 2) {
				if (setupClient() != 0) {
					printf("There was an error setting up the client.\n");
					break;
				}
				connectToServer();
				SDL_Delay(500);

				// Recieves info from the server and parses it

				playerTexture = loadImage(renderer, "textures/OtherPlayer.png");
				playerTextureCrouch = loadImage(renderer, "textures/OtherPlayerCrouch.png");

				loadMap("levels/level1.map");
				onServer = true;
				paused = false;
			}
			renderMenu();
		}
		renderToasts();
		SDL_RenderPresent(renderer);
		calculateDT();
		//calculateFPS();
	}

	if (onServer) {
		printf("Disconnecting.\n");

		// Disconnect message kills all the threads
		disconnectPacket("Game shutting down");

		int returnVal;
		// Wait for threads to return
		playing = false;
		SDL_WaitThread(getUpdate, &returnVal);
		printf("Disconnected.\n");
		zed_net_shutdown();
	}

	printf("Saving settings.\n");
	if (saveSettings() != 0) {
		printf("Failed to save settings. All changes will be lost.\nSorry.");
	}

	printf("Closing.\n");

	// Destroys all the textures
	for (size_t x = 0; x < mapSize; x++) {
		SDL_DestroyTexture(map[x].tex);
	}
	for (size_t x = 0; x < interactableMapSize; x++) {
		SDL_DestroyTexture(interactableMap[x].activeTex);
		SDL_DestroyTexture(interactableMap[x].inactiveTex);
	}
	for (size_t x = 0; x < textArrSize; x++) {
		SDL_DestroyTexture(textArr[x].tex);
	}
	if (onServer) {
		SDL_DestroyTexture(playerTexture);
		SDL_DestroyTexture(playerTextureCrouch);
	}
	SDL_DestroyTexture(MainPlayer.tex);
	SDL_DestroyTexture(MainPlayer.crouchTex);

	// Destroy the window.
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0; 
}

