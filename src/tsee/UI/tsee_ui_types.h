// A child to a top-level toolbar button.
typedef struct {
	TSEE_Object *text;
	void (*callback) (void *tsee);
} TSEE_Toolbar_Child;

// Top-level button on the toolbar.
typedef struct TSEE_Toolbar_Object {
	TSEE_Object *text;
	TSEE_Array *buttons; // Array of TSEE_Toolbar_Child
	bool expanded;
} TSEE_Toolbar_Object;

// TSEE's UI struct. Keeps track of all UI elements and what's enabled.
typedef struct TSEE_UI {
	TSEE_Array *toolbar; // Array of TSEE_Toolbar_Object
	bool toolbar_enabled;
} TSEE_UI;