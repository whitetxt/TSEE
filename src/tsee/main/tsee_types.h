// !! TSEE TYPES !!

// Struct to keep track of all the world's objects.
typedef struct TSEE_World {
	TSEE_Array *objects;
	float gravity;
	float scroll_x;
	float scroll_y;
	float max_scroll_x;
} TSEE_World;

// TSEE's system of keeping track of what's initialized.
typedef struct TSEE_Init {
	bool rendering;
	bool text;
	bool events;
	bool input;
	bool ui;
} TSEE_Init;

typedef struct TSEE_Debug {
	double event_time;
	double physics_time;
	double render_time;
	double frame_time;
	double framerate;
	bool active;
} TSEE_Debug;

// The main TSEE object, create using TSEE_Create(width, height).
typedef struct TSEE {
	TSEE_Window *window;
	TSEE_Array *textures;
	TSEE_Array *fonts;
	TSEE_World *world;
	TSEE_Events *events;
	TSEE_Player *player;
	TSEE_Init *init;
	TSEE_UI *ui;
	TSEE_Debug *debug;
	Uint64 last_time;
	Uint64 current_time;
	float dt;
} TSEE;