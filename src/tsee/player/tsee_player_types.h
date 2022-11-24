// Struct to keep track of which way the player is moving
typedef struct TSEE_Player_Movement {
	bool up;
	bool down;
	bool left;
	bool right;
} TSEE_Player_Movement;

// TSEE's player type, which extends TSEE_Physics_Object.
typedef struct TSEE_Player {
	TSEE_Object *object;
	TSEE_Player_Movement movement;
	bool grounded;
	double held_up;
	double jump_force;
	double speed;
	int step_size;
} TSEE_Player;