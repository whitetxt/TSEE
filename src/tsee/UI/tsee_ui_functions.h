// Initializes the UI system.
// MUST be called after TSEEInitText
bool TSEEInitUI(TSEE *tsee);

// Adds a button to the top-level toolbar.
bool TSEEAddToolbarButton(TSEE *tsee, char *font, char *text);

// Adds a child button to a parent button.
bool TSEEAddToolbarChild(TSEE *tsee, char *parentName, char *font, char *text, void (*cb) (void *tsee));

// Checks if the UI was clicked
bool TSEEUIClick(TSEE *tsee, int x, int y);

// Renders all toolbar buttons.
bool TSEERenderUI(TSEE *tsee);