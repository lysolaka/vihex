// Enum for printCout
enum coutFormat { NORMAL, QUIET, NORMAL_FULL, QUIET_FULL };

// Function prototypes
void printHelp();
int cursesEditor(char *filename);
int printCout(char *filename, coutFormat format);
