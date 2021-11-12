#include "include/main.h"

size_t inputNum;

Menu *menu = NULL;

SDL_Color Selected = {49, 127, 67, 255};
SDL_Color Black = {0, 0, 0, 255};

char *menuFrom;
char *currentMenu;

SDL_Event menuEv;
bool inputting = false;
char finalText[512];
bool pressed = false;
char *lastKey = "¬";

// Creates a new menu from a file
// @param path The path to the menu file minus the menus/ prefix.
// @return 0 on success, 1 on failure
int createMenu(char *path) {
	// Open a menu file by path
	char outpath[256];
	sprintf(outpath, "menus/%s", path); // Format the file to the correct directory
	FILE *fp = fopen(outpath, "r"); // Open the file
	menuFrom = strdup(currentMenu);
	currentMenu = strdup(path);
	menu = malloc(sizeof(Menu)); // Allocate memory for the menu
	menu->items = malloc(sizeof(MenuItem)); // Allocate memory for the menu items
	menu->numItems = 0;
	menu->selected = 0;

	if (!fp) { // If the file doesnt exist
		printf("Failed to open menu file: menus/%s\n", outpath);
		return 1;
	}

	char buf[2048];
	bool found = false;

	while (fgets(buf, sizeof(buf), fp)) { // While there is a line
		buf[strcspn(buf, "\n")] = 0;
		menu->items = realloc(menu->items, ++menu->numItems * sizeof(*menu->items)); // Increase the size of the menu array
		struct MenuItem tmpItem;
		tmpItem.name = strdup(strtok(buf, ":"));
		tmpItem.nextmenu = strdup(strtok(NULL, ":"));
		tmpItem.textInput = false;
		if (strcmp(tmpItem.name, "UNPAUSABLE") == 0) { // If this menu is unpausable
			unpausable = true;
			menu->numItems--;
			continue;
		} else if (strcmp(tmpItem.name, "NOT_UNPAUSABLE") == 0) { // If this menu is not unpausable
			unpausable = false;
			menu->numItems--;
			continue;
		}
		if (strcmp(tmpItem.nextmenu, "NULL") == 0) { // If the destination is set to have no destination
			printf("WARN: No destination menu for option %s\n", tmpItem.name);
		}
		if (strcmp(tmpItem.name, "TITLE") == 0) { // If the text found is a title
			if (found) {
				printf("Multiple titles found. Existing title: %s\nNew title: %s\nUsing newer title.", menu->menuTitle.nextmenu, tmpItem.nextmenu);
			}
			// Revert the changes made
			menu->numItems--;
			// Set the title
			tmpItem.selected = createColouredText(tmpItem.nextmenu, width / 2, 20, titleFont, Black);
			menu->menuTitle = tmpItem;
			found = true;
			continue;
		} else if (strcmp(tmpItem.nextmenu, "LoadLevel") == 0) { // If the item loads a map
			tmpItem.levelName = strdup(strtok(NULL, ":")); // Set the map to load
		} else if (strcmp(tmpItem.nextmenu, "TextInput") == 0) { // If its an input box
			tmpItem.textInput = true;
			tmpItem.text = '\0';
			tmpItem.textBuf[0] = '\0';
		} 
		if (strcmp(menu->menuTitle.nextmenu, "Settings") == 0) {
			if (strcmp(tmpItem.name, "Multiplayer Name") == 0) {
				tmpItem.text = strdup(Settings.name);
				strcat(tmpItem.textBuf, Settings.name);
			}
		}
		tmpItem.unselected = createColouredText(tmpItem.name, 20, (28 * menu->numItems) + (menu->menuTitle.selected.rect.h + 25), menuFont, Black);
		tmpItem.selected = createColouredText(tmpItem.name, 20, (28 * menu->numItems) + (menu->menuTitle.selected.rect.h + 25), menuFont, Selected);
		menu->items[menu->numItems - 1] = tmpItem;
	}
	if (!found) {
		// If there is no title text
		printf("Title for menu not found.\n");
		menu->menuTitle.nextmenu = "TITLE NOT FOUND";
	}
	menu->selected = 0;
	return 0;
}

// Renders the menu
void renderMenu() {
	// Special function for rendering the menu.
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	Text tmpText;

	// Render the title
	menu->menuTitle.selected.rect.x = width / 2 - menu->menuTitle.selected.rect.w / 2;
	SDL_RenderCopy(renderer, menu->menuTitle.selected.tex, NULL, &menu->menuTitle.selected.rect);
	int offset = menu->menuTitle.selected.rect.h + 20;

	// Render the seperation bar
	SDL_Rect tmpRect;
	tmpRect.x = 0;
	tmpRect.y = offset + 5;
	tmpRect.w = width;
	tmpRect.h = 2;
	SDL_RenderDrawRect(renderer, &tmpRect);

	for (size_t x = 0; x < menu->numItems; x++) {
		// Create the text for this screen
		if (x == menu->selected)
			SDL_RenderCopy(renderer, menu->items[x].selected.tex, NULL, &menu->items[x].selected.rect);
		else
			SDL_RenderCopy(renderer, menu->items[x].unselected.tex, NULL, &menu->items[x].unselected.rect);

		if (menu->items[x].textInput) { // If the text is an input area
			// Create the text input box
			tmpRect.x = menu->items[x].unselected.rect.x + menu->items[x].unselected.rect.w + 10;
			tmpRect.y = menu->items[x].unselected.rect.y - 1;
			tmpRect.w = width - menu->items[x].unselected.rect.x - menu->items[x].unselected.rect.w - 20;
			tmpRect.h = menu->items[x].unselected.rect.h + 2;
			// Render the box
			SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);
			SDL_RenderDrawRect(renderer, &tmpRect);
			if (menu->items[x].text != NULL && strcmp(menu->items[x].text, "") != 0) { // If there is text in the box
				// Render the text
				tmpText = createColouredText(menu->items[x].text, tmpRect.x + 5, tmpRect.y + 1, menuFont, Black);
				SDL_RenderCopy(renderer, tmpText.tex, NULL, &tmpText.rect);
				freeText(tmpText);
			}
			if (x == menu->selected) {
				// Var used to track keyboard inputs
				inputting = true;
			}
		}
	}
	return;
}

// Selects the option
// @return 0 to do nothing, 1 to quit the game, 2 to connect to a server.
int selectOption() {
	// Get the menu to go to or whatever else is in that field.
	char *menuToGo = menu->items[menu->selected].nextmenu;
	if (strcmp(menuToGo, "ResumeGame") == 0) { // Special command to resume the game
		paused = false;
		return 0;
	} else if (strcmp(menuToGo, "ExitGame") == 0) { // Special command to quit the game
		return 1;
	} else if (strcmp(menuToGo, "NULL") == 0) { // If it has no destination
		printf("%s has no menu destination.\n", menu[menu->selected].name);
		return 0;
	} else if (strcmp(menuToGo, "LoadLevel") == 0) { // Special command to load a level instead of a menu
		if (loadMap(menu->items[menu->selected].levelName) != 0) {
			printf("Failed to load into level: %s\n", menu->items[menu->selected].levelName);
			return 0;
		} else {
			paused = false;
			unpausable = true;
			return 0;
		}
	} else if (strcmp(menuToGo, "MenuBack") == 0) { // Special command to allow for menus to be used in multiple places, while returning to different menus
		menuToGo = strdup(menuFrom);
	} else if (strcmp(menuToGo, "TextInput") == 0) { // Special command to input into text field
		inputting = true;
		return 0;
	} else if (strcmp(strtok(menuToGo, ","), "ServerConnect") == 0) { // Special command to connect to server
		return 2;
	} else if (strcmp(menuToGo, "ApplySettings") == 0) {
		for (size_t x = 0; x < menu->numItems; x++) {
			if (strtok(menu->items[x].name, "Multiplayer Name") == 0) {
				Settings.name = strdup(menu->items[x].text);
			}
		}
		menuToGo = strdup(menuFrom); // Go back a menu
		printf("Settings saved!\n");
		createToast("Settings saved! And I've made this longer now!");
	}
	createMenu(menuToGo);
	return 0;
}

// Menu event handler
void menuInputHandling() {
	// Special event handling to handle key inputs
	switch (event.type) {
		case SDL_TEXTINPUT:
			// Handle text entering
			if (inputting && (event.text.text != lastKey) && (strcmp(event.text.text, " ") != 0)) {
				strcat(menu->items[menu->selected].textBuf, event.text.text);
				menu->items[menu->selected].text = strdup(menu->items[menu->selected].textBuf);
				lastKey = strdup(event.text.text);
			}
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
				// Handle backspace
				case SDLK_BACKSPACE:
					if (strlen(menu->items[menu->selected].text) != 0)
						menu->items[menu->selected].text[strlen(menu->items[menu->selected].text) - 1] = '\0';
					if (strlen(menu->items[menu->selected].textBuf) != 0)
						menu->items[menu->selected].textBuf[strlen(menu->items[menu->selected].textBuf) - 1] = '\0';
			}
	}
}