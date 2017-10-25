#ifndef mainTools_h
#define mainTools_h

char * getColumnHeader(int argc, char ** argv);
char * getInputDirectory(int argc, char ** argv);
char * getOutputDirectory(int argc, char ** argv);
void freeTable(char * *** table, int rows, int columns);

#endif /* mainTools_h */
