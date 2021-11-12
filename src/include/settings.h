typedef struct configuration {
	const char* name;
	int playerTextSize;
	int width;
	int height;
} configuration;

extern configuration Settings;

int loadSettings();
int saveSettings();