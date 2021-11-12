typedef struct {
	char *text;
	Text textObj;
} Toast;

bool createToast(char *text);
void renderToasts();