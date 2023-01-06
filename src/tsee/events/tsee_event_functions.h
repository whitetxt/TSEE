// Events

bool TSEE_Events_Init(TSEE *tsee);
bool TSEE_Events_Handle(TSEE *tsee);

// Input

bool TSEE_Input_Init(TSEE *tsee);
void TSEE_Key_Press(void *tsee, SDL_Keycode key);
void TSEE_Key_Release(void *tsee, SDL_Keycode key);