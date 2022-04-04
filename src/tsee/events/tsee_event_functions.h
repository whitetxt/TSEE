// Initializes events for a TSEE
bool TSEE_Events_Init(TSEE *tsee);

// Handles events for a TSEE and calls the callbacks for each event.
bool TSEE_Events_Handle(TSEE *tsee);

// INPUT

// Initializes TSEE input and includes some basic functions for movement of the player.
bool TSEE_Input_Init(TSEE *tsee);

// Basic key press and release function.
void TSEE_Key_Press(void *tsee, SDL_Keycode key);

// Basic key press and release function.
void TSEE_Key_Release(void *tsee, SDL_Keycode key);