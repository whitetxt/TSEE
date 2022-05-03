// Maps

bool TSEE_ReadFile(void *dst, size_t size, size_t n, FILE *fp);
bool TSEE_WriteFile(void *src, size_t size, size_t n, FILE *fp);
char *TSEE_ReadFile_UntilNull(FILE *fp);
void TSEE_WriteFile_String(FILE *fp, char *string);
bool TSEE_Map_Load(TSEE *tsee, char *path);
bool TSEE_Map_Save(TSEE *tsee, char *path);