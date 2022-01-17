// !! PHYSICS TYPES !!

// TSEE's 2D Vector type.
typedef struct TSEE_Vec2 {
	double x;
	double y;
} TSEE_Vec2;

// !! TSEE TYPES !!

// TSEE's window type wrapper for SDL2, created along with the TSEE object in TSEECreate(width, height).
typedef struct TSEE_Window {
	SDL_Window *window;
	SDL_Renderer *renderer;
	int width;
	int height;
	int fps;
	bool running;
	char *title;
	SDL_Point mouse;
} TSEE_Window;

// TSEE fonts, stores the font, its name and size.
typedef struct TSEE_Font {
	TTF_Font *font;
	char *name;
	int size;
} TSEE_Font;

// TSEE's home-made array type. Stores the current size & data.
// Also has helper functions to insert, delete, append and more.
typedef struct TSEE_Array {
	void **data;
	size_t size;
} TSEE_Array;

// TSEE's texture wrapper, keeps track of important information for the texture.
typedef struct TSEE_Texture {
	SDL_Texture *texture;
	SDL_Rect rect;
	char *path;
} TSEE_Texture;

// TSEE's text wrapper, keeps track of what the text says.
typedef struct TSEE_Text {
	TSEE_Texture *texture;
	char *text;
} TSEE_Text;

// TSEE's Object type, keeps track of texture and position.
typedef struct TSEE_Object {
	TSEE_Texture *texture;
	double x;
	double y;
} TSEE_Object;

// TSEE's Physics Object type, extends TSEE_Object and has physics related information.
typedef struct TSEE_Physics_Object {
	TSEE_Object *object;
	TSEE_Vec2 velocity;
	TSEE_Vec2 acceleration;
	TSEE_Vec2 force;
	double mass;
} TSEE_Physics_Object;

// Struct to keep track of all the world's objects.
typedef struct TSEE_World {
	TSEE_Array *objects;
	TSEE_Array *physics_objects;
	TSEE_Array *text;
	TSEE_Array *parallax;
	double gravity;
	double scroll_x;
	double scroll_y;
	double max_scroll_x;
} TSEE_World;

// Struct to keep track of which way the player is moving
typedef struct TSEE_Player_Movement {
	bool up;
	bool down;
	bool left;
	bool right;
} TSEE_Player_Movement;

// Struct for parallax scrolling backgrounds
typedef struct TSEE_Parallax {
	TSEE_Texture *texture;
	double distance;
} TSEE_Parallax;

// TSEE's player type, which extends TSEE_Physics_Object.
typedef struct TSEE_Player {
	TSEE_Physics_Object *physics_object;
	TSEE_Player_Movement movement;
	bool grounded;
	double jump_force;
	double speed;
} TSEE_Player;

// TSEE's event handling system. 
typedef struct TSEE_Events {
	SDL_Event *event;
	void (*keypress) (void *tsee, SDL_Keycode keycode);
	void (*keyrelease) (void *tsee, SDL_Keycode keycode);
	void (*mousemotion) (void *tsee, int x, int y);
	void (*mouseclick) (void *tsee, int x, int y, int button);
} TSEE_Events;

// TSEE's system of keeping track of what's initialized.
typedef struct TSEE_Init {
	bool rendering;
	bool text;
	bool events;
	bool input;
	bool ui;
} TSEE_Init;

// A child to a top-level toolbar button.
typedef struct {
	TSEE_Text *text;
	void (*callback) (void *tsee);
} TSEE_Toolbar_Child;

// Top-level button on the toolbar.
typedef struct TSEE_Toolbar_Object {
	TSEE_Text *text;
	TSEE_Array *buttons; // Array of TSEE_Toolbar_Child
	bool expanded;
} TSEE_Toolbar_Object;

// TSEE's UI struct. Keeps track of all UI elements and what's enabled.
typedef struct TSEE_UI {
	TSEE_Array *toolbar; // Array of TSEE_Toolbar_Object
} TSEE_UI;

// The main TSEE object, create using TSEECreate(width, height).
typedef struct TSEE {
	TSEE_Window *window;
	TSEE_Array *textures;
	TSEE_Array *fonts;
	TSEE_World *world;
	TSEE_Events *events;
	TSEE_Player *player;
	TSEE_Init *init;
	TSEE_UI *ui;
	Uint64 last_time;
	Uint64 current_time;
	double dt;
} TSEE;