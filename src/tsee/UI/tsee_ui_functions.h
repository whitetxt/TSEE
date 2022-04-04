// Initializes the UI system.
// MUST be called after TSEE_Text_Init
bool TSEE_UI_Init(TSEE *tsee);

// Adds a button to the top-level toolbar.
bool TSEE_Toolbar_AddButton(TSEE *tsee, char *font, char *text);

// Adds a child button to a parent button.
bool TSEE_Toolbar_AddChild(TSEE *tsee, char *parentName, char *font, char *text, void (*cb) (void *tsee));

// Checks if the UI was clicked
bool TSEE_UI_Click(TSEE *tsee, int x, int y);

// Renders all toolbar buttons.
bool TSEE_UI_Render(TSEE *tsee);