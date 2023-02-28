// Maps

bool TSEE_ReadFile(void *dst, size_t size, size_t n, FILE *fp);
bool TSEE_WriteFile(void *src, size_t size, size_t n, FILE *fp);

char *TSEE_ReadFile_UntilNull(FILE *fp);
TSEE_Vec2 *TSEE_ReadFile_Vec2(FILE *fp);

void TSEE_WriteFile_String(FILE *fp, char *string);
void TSEE_WriteFile_Vec2(FILE *fp, TSEE_Vec2 vec2);

bool TSEE_Map_Load(TSEE *tsee, char *path);
bool TSEE_Map_Save(TSEE *tsee, char *path);