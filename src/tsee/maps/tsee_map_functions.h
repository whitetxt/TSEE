// Internal helper function to read from a file until it hits a null char.
char *TSEEReadFileUntilNull(FILE *fp);

// Loads a map from a file.
bool TSEELoadMap(TSEE *tsee, char *path);

// Saves a map to a file.
bool TSEESaveMap(TSEE *tsee, char *path);