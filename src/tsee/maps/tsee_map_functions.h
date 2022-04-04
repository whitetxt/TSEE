// Internal helper function to read from a file until it hits a null char.
char *TSEE_ReadFile_UntilNull(FILE *fp);

// Loads a map from a file.
bool TSEE_Map_Load(TSEE *tsee, char *path);

// Saves a map to a file.
bool TSEE_Map_Save(TSEE *tsee, char *path);