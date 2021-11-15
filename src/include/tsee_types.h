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
} TSEE_Window;

// The main TSEE object, create using TSEECreate(width, height).
typedef struct TSEE {
	TSEE_Window *window;
} TSEE;

// TSEE's texture wrapper, keeps track of important information for the texture.
typedef struct TSEE_Texture {
	SDL_Texture *texture;
	int height;
	int width;
} TSEE_Texture;

// TSEE's Object type, keeps track of texture and position.
typedef struct TSEE_Object {
	TSEE_Texture *texture;
	int x;
	int y;
} TSEE_Object;

// TSEE's Physics Object type, extends TSEE_Object and has physics related information.
typedef struct TSEE_Physics_Object {
	TSEE_Object *object;
	TSEE_Vec2 velocity;
	TSEE_Vec2 acceleration;
	TSEE_Vec2 force;
} TSEE_Physics_Object;