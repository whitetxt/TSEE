#include "include/main.h"

SDL_Event event;
bool spaceHeld = false;
bool escHeld = false;
bool downHeld = false;
bool upHeld = false;
bool unpausable = false;
Player *player;

int eventHandling(Player *Pl) {
	player = Pl;
	// Using SDL_GetKeyboardState because that allows for held characters easily. SDL_KEYDOWN event does not work like this.
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	// Handling movement
	if (keys[SDL_SCANCODE_W]) 
		playerJump(player);
	
	if (keys[SDL_SCANCODE_A]) {
		movePlayer(player, 3);
	}
	
	if (keys[SDL_SCANCODE_S]) {
		if (!player->isCrouching)
			startCrouch(player);
	} else {
		if (player->isCrouching)
			endCrouch(player);
	}
	
	if (keys[SDL_SCANCODE_D]) {
		movePlayer(player, 1);
	}

	// Polling events
	

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			// If the window has been closed
			case SDL_QUIT:
				return 1;
			
			// If key pressed: Used here for single press keys.
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						if (!escHeld) {
							paused = true;
							escHeld = true;
							unpausable = true;
							createMenu("pause.menu");
						}
						break;
					case SDLK_SPACE:
						if (!spaceHeld) {
							interactables(player);
							spaceHeld = true;
						}
						break;
					default:
						break;
				}
				break;
			
			case SDL_KEYUP:
				switch (event.key.keysym.sym) {
					case SDLK_SPACE:
						spaceHeld = false;
						break;
					case SDLK_ESCAPE:
						escHeld = false;
						break;
				}
				break;
			
			// Allows for the window to be resized, and to still function correctly; although resizing isnt recommended.
			case SDL_WINDOWEVENT:
				switch (event.window.event) {
					case SDL_WINDOWEVENT_RESIZED:
						width = event.window.data1;
						height = event.window.data2;
						Settings.width = width;
						Settings.height = height;
						break;
				}
				break;
		}
	}
	return 0;
}

int pauseEventHandling() {
	// Polling events
	while (SDL_PollEvent(&event)) {
		menuInputHandling();

		switch (event.type) {
			// If the window has been closed
			case SDL_QUIT:
				return 1;
			
			// If key pressed: Used here for single press keys.
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						if (!escHeld && unpausable) {
							paused = false;
							escHeld = true;
						}
						break;
					case SDLK_RETURN:
					case SDLK_SPACE:
						if (!spaceHeld) {
							int retVal = selectOption();
							if (retVal == 1)
								return 1;
							else if (retVal == 2)
								return 2;
							spaceHeld = true;
						}
						break;
					case SDLK_DOWN:
						if (!downHeld) {
							menu->selected++;
							if (menu->selected >= menu->numItems) {
								menu->selected = 0;
							}
							downHeld = true;
						}
						break;
					case SDLK_UP:
						if (!upHeld) {
							if (menu->selected == 0) {
								menu->selected = menu->numItems;
							}
							menu->selected--;
							upHeld = true;
						}
						break;
					default:
						break;
				}
				break;
			
			case SDL_KEYUP:
				pressed = false;
				switch (event.key.keysym.sym) {
					case SDLK_RETURN:
					case SDLK_SPACE:
						spaceHeld = false;
						break;
					case SDLK_ESCAPE:
						escHeld = false;
						break;
					case SDLK_UP:
						upHeld = false;
						break;
					case SDLK_DOWN:
						downHeld = false;
						break;
				}
				break;
			
			// Allows for the window to be resized, and to still function correctly; although resizing isnt recommended.
			case SDL_WINDOWEVENT:
				switch (event.window.event) {
					case SDL_WINDOWEVENT_RESIZED:
						width = event.window.data1;
						height = event.window.data2;
						Settings.width = width;
						Settings.height = height;
						break;
				}
				break;
		}
	}
	return 0;
}