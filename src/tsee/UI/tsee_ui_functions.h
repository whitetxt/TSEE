// UI

bool TSEE_UI_Init(TSEE *tsee);
bool TSEE_Toolbar_AddButton(TSEE *tsee, char *font, char *text);
bool TSEE_Toolbar_AddChild(TSEE *tsee, char *parentName, char *font, char *text, void (*cb) (void *tsee));
bool TSEE_UI_Click(TSEE *tsee, int x, int y);
bool TSEE_UI_Render(TSEE *tsee);