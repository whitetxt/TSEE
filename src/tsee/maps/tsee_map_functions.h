// Maps

char *TSEE_ReadFile_UntilNull(FILE *fp);
bool TSEE_Map_Load(TSEE *tsee, char *path);
bool TSEE_Map_Save(TSEE *tsee, char *path);