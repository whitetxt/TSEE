// Initializes events for a TSEE
bool TSEEInitEvents(TSEE *tsee);

// Handles events for a TSEE and calls the callbacks for each event.
bool TSEEHandleEvents(TSEE *tsee);

// INPUT

// Initializes TSEE input and includes some basic functions for movement of the player.
bool TSEEInitInput(TSEE *tsee);

// Basic key press and release function.
void TSEEKeyPress(void *tsee, SDL_Keycode key);

// Basic key press and release function.
void TSEEKeyRelease(void *tsee, SDL_Keycode key);