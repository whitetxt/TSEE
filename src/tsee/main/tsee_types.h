// !! TSEE TYPES !!

// Struct to keep track of all the world's objects.
typedef struct TSEE_World {
	TSEE_Array *objects;
	TSEE_Vec2 gravity;
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
	bool resources;
} TSEE_Init;

typedef struct TSEE_Debug_RenderTimes {
	double object_time;
	double parallax_time;
} TSEE_Debug_RenderTimes;

typedef struct TSEE_Debug {
	double event_time;
	double physics_time;
	double render_time;
	TSEE_Debug_RenderTimes render_times;
	double frame_time;
	double framerate;
	bool active;
} TSEE_Debug;

// The main TSEE object, create using TSEE_Create(width, height).
typedef struct TSEE {
	TSEE_Window *window;
	// Moved to using a resource manager.
	// TSEE_Array *textures;
	// TSEE_Array *fonts;
	TSEE_ResManager *resources;
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