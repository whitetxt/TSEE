typedef struct MenuItem {
	char *name;
	char *nextmenu;
	char *levelName;
	char *text;
	char textBuf[512];
	bool textInput;
	Text unselected;
	Text selected;
} MenuItem;

typedef struct Menu {
	char *name;
	MenuItem *items;
	size_t numItems;
	size_t selected;
	MenuItem menuTitle;
} Menu;

extern size_t inputNum;

extern Menu *menu;

extern SDL_Color Selected;
extern SDL_Color Black;

extern bool pressed;

int createMenu();
void renderMenu();
int selectOption();
void menuInputHandling();