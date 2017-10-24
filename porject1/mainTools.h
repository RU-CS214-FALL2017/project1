#ifndef mainTools_h
#define mainTools_h

char * getFlagValue(const char * flag, int argc, char ** argv);
char * getFlagValueHelper(int flagIndex, int argc, char ** argv);
int findFlag(const char * flag, int argc, char ** argv);
void freeTable(char * *** table, int rows, int columns);

#endif /* mainTools_h */
