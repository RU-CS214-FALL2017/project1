#ifndef tools_h
#define tools_h

#include <stdio.h>
#include <sys/types.h>

#define TEMPSIZE 4096

// Represents a processed directory.
struct csvDir {
    
    pid_t pid;
    char * path;
    
    pid_t * subChildPids;
    struct csvDir * subDirs;
    unsigned int * numSubDirs;
    
    pid_t * csvChildPids;
    char ** csvPaths;
    unsigned int * numCsvPaths;
};

unsigned int tokenizeRow(const char * line, char * ** row);
void trim (char * str);
void removeChars (char * str, unsigned long startIndex, unsigned long endIndex);
void fillTable(FILE * csvFile, char * *** table, unsigned int * rows, unsigned int * columns);
void printTable (FILE * stream, char *** table, unsigned int rows, unsigned int columns);
int isNumber(const char * str);
int isXBeforeY (const char * x, const char * y, int areNumbers);
int isNumericColumn(char *** table, int rows, int columnIndex);
int findCsvFiles(const char * dirPath, char * ** csvPaths, int * numFound);
int isProperCsv(const char * csvPath);
unsigned int lineageParser(const char * path, char * ** lineage);
pid_t * multiFork(int num);
void * pipeDataToChildren(const void * source, size_t size, int pipedFd[2], int isParent, int numChildren);
void * myMap(size_t size);
void * myReMap(void * address, size_t oldSize, size_t newSize);

#endif /* tools_h */
