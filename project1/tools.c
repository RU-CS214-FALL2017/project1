#define _GNU_SOURCE 1
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>

#include "tools.h"
#include "sorter.h"

int findCsvFilesHelper(const char * dirPath, char ** csvPaths, int * numFound);

// <row> is the address to a char **. Creates a array of strings
// A, where each comma seperated value from <line> is an element
// of A, and <row>'s refrence is set to point to A. Returns the
// number elements in A (columns). To free, free each (*<row>)[i]
// (0 <= i < # of elements in A) and free *<row>.
unsigned int tokenizeRow(const char * line, char * ** row) {
    
    *row = (char **) malloc(strlen(line) * sizeof(char *));
    
    char tempChar = '\0';
    char tempCell[TEMPSIZE];
    int i = 0; // number of columns
    int j = 0; // number of characters in column field
    int inQuote = 0;
    int outQuote = 0;
    
    for(int l = 0; l <= strlen(line); l ++) {
        
        tempChar = line[l];
        
        if ((tempChar == ',' || tempChar == '\0') && !inQuote){
            
            tempCell[j] = '\0';
            trim(tempCell);
            (*row)[i] = (char *) malloc(strlen(tempCell) * sizeof(char) + 1);
            strcpy((*row)[i], tempCell);
            
            j = 0;
            i += 1;
            
            outQuote = 0;
            
        } else if (tempChar == '"' && inQuote) {
            inQuote = 0;
            
        } else if (tempChar == '"' && !outQuote && !inQuote) {
            inQuote = 1;
            
        } else {
            
            tempCell[j] = tempChar;
            j += 1;
            if(tempChar != ' '){
                outQuote = 1;
            }
        }
    }
    
    *row = (char **) realloc(*row, i * sizeof(char *));
    
    return i;
}

// Removes leading and trailing whitespaces from <str>.
void trim (char * str) {
    
    int i = 0;
    
    while(str[i] == ' ' || str[i] == '\n' || str[i] == '\t' || str[i] == '\r' || str[i] == '\v'
          || str[i] == '\f') {
        i += 1;
    }
    
    removeChars(str, 0, i);
    
    unsigned long terminatingIndex = strlen(str);
    unsigned long j = terminatingIndex - 1;
    
    while(str[j] == ' ' || str[j] == '\n' || str[j] == '\t' || str[j] == '\r' || str[j] == '\v'
          || str[j] == '\f') {
        j -= 1;
    }
    
    removeChars(str, j + 1, terminatingIndex);
}

// Removes the characters from <str> between indexes <startIndex> (inclusive)
// and <endIndex> (exclusive).
void removeChars (char * str, unsigned long startIndex, unsigned long endIndex) {
    
    unsigned long terminatingIndex = strlen(str);
    
    for(int i = 0; i <= (terminatingIndex - endIndex); i++) {
        
        str[startIndex + i] = str[endIndex + i];
    }
}

// <table> is an address to a char ***. <rows> is an address to
// an unsigned int. <columns> is an address to an unsigned int.
// Creates a "table" from <csvFile> as a 2D string array, where
// A[n][m] will return a string representation of the data stored
// in the (n+1)th row and (m+1)th column of the "table". <table>'s
// refrence is set to point to the created "table". <rows>' refrence
// is set to the numbers of rows in "table". <columns>' refrence is
// set to the number of columns in "table". To free, free each
// (*<table>)[i][j] (0 <= i < *<rows>, 0 <= j < *<columns>) and free
// *<table>.
void fillTable(FILE * csvFile, char * *** table, unsigned int * rows, unsigned int * columns) {
    
    *table = (char ***) malloc(4194304 * sizeof(char **));
    char line[TEMPSIZE];
    *rows = 0;
    *columns = 0;
    
    while(fgets(line, TEMPSIZE, csvFile) != NULL) {
        
        int tempColumns = tokenizeRow(line, &(*table)[*rows]);
        
        if (*rows == 0) {
            
            *columns = tempColumns;
            (*rows)++;
            
        } else if (tempColumns == *columns) {
            (*rows)++;
            
        } else {
            
            for (int j = 0; j < tempColumns; j++) {
                free((*table)[*rows][j]);
            }
            
            free((*table)[*rows]);
        }
    }
    
    *table = (char ***) realloc(*table, sizeof(char **) * *rows);
}

// Prints <table> with <rows> rows and <columns> columns in a
// csv (comma seperated values) format to <stream>.
void printTable (FILE * stream, char *** table, unsigned int rows, unsigned int columns) {
    
    for (int i = 0; i < rows; i++) {
        
        if(strchr(table[i][0], ',') != NULL) {
            fprintf(stream, "\"%s\"", table[i][0]);
            
        } else {
            fprintf(stream, "%s", table[i][0]);
        }
        
        for (int j = 1; j < columns; j++) {
            
            if(strchr(table[i][j], ',') != NULL) {
                fprintf(stream, ",\"%s\"", table[i][j]);
                
            } else {
                fprintf(stream, ",%s", table[i][j]);
            }
        }
        
        fprintf(stream, "\n");
    }
}

// Returns 1 if <str> is a valid number, else returns 0.
int isNumber(const char * str) {
    
    for (int i = 0; i < strlen(str); i++) {
        
        if (!(isdigit(str[i]) || (str[i] == '.'))) {
            return 0;
        }
    }

    return 1;
}

// If <areNumbers> is set to 0, returns 0 if <y> is lexicographically before <x>,
// else returns 1. If <areNumbers> is set to anything besides 0, converts <x> and
// <y> to double values x y respectively, and returns x <= y.
int isXBeforeY (const char * x, const char * y, int areNumbers) {
    
    if (areNumbers) {
        return atof(x) <= atof(y);
        
    } else {
        return (strcmp(x, y) <= 0);
    }
}

// If all the values in <table> at column index <columnIndex> from row index 1
// to row index <rows> - 1 are valid numbers, returns 1, else returns 0.
int isNumericColumn(char *** table, int rows, int columnIndex) {
    
    for (int i = 1; i < rows; i++) {
        
        if (!isNumber(table[i][columnIndex])) {
            return 0;
        }
    }
    
    return 1;
}

int processCsvDir(const char * path, struct csvDir * info, const char * columnHeader, const char * outputDir) {
    
    DIR * dir = opendir(path);
    
    if (dir == NULL) {
        return 0;
    }
    
    info = (struct csvDir *) myMap(sizeof(struct csvDir));
    
    info->pid = getpid();
    info->path = myMap(strlen(path));
    strcpy(info->path, path);
    
    info->subChildPids = (pid_t *) myMap(sizeof(pid_t) * TEMPSIZE);
    info->subDirs = (struct csvDir *) myMap(sizeof(struct csvDir) * TEMPSIZE);
    info->numSubDirs = myMap(sizeof(unsigned int));
    *(info->numSubDirs) = 0;
    
    info->csvChildPids = (pid_t *) myMap(sizeof(pid_t) * TEMPSIZE);
    info->csvPaths = (char **) myMap(sizeof(char *) * TEMPSIZE);
    info->numCsvPaths = myMap(sizeof(unsigned int));
    *(info->numCsvPaths) = 0;
    
    for (struct dirent * entry = readdir(dir); entry != NULL; entry = readdir(dir)) {
        
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            
            char subDirPath[TEMPSIZE];
            sprintf(subDirPath, "%s/%s", path, entry->d_name);
            
            (info->subChildPids)[*(info->numSubDirs)] = fork();
            
            if ((info->subChildPids)[*(info->numSubDirs)] == 0) {
            
                processCsvDir(subDirPath, &((info->subDirs)[*(info->numSubDirs)]), columnHeader, outputDir);
                (*(info->numSubDirs))++;
            }
            
        } else if (entry->d_type == DT_REG) {
            
            char csvPath[TEMPSIZE];
            sprintf(csvPath, "%s/%s", path, entry->d_name);
            
            (info->csvChildPids)[*(info->numCsvPaths)] = fork();
            
            if ((info->csvChildPids)[*(info->numCsvPaths)] == 0) {
                
                if (isProperCsv(csvPath)) {
                    
                    (info->csvPaths)[*(info->numCsvPaths)] = myMap(strlen(csvPath) + 1);
                    strcpy((info->csvPaths)[*(info->numCsvPaths)], csvPath);
                    (*(info->numCsvPaths))++;
                    
                    if (outputDir == NULL) {
                        sortCsv(csvPath, columnHeader, path);
                        
                    } else {
                        sortCsv(csvPath, columnHeader, outputDir);
                    }
                }
            }
        }
    }
    
    info->subChildPids = (pid_t *) myReMap(info->subChildPids, sizeof(pid_t) * TEMPSIZE, sizeof(pid_t) * *(info->numSubDirs));
    info->subDirs = (struct csvDir *) myReMap(info->subDirs, sizeof(struct csvDir) * TEMPSIZE, sizeof(pid_t) * *(info->numSubDirs));
    
    info->csvChildPids = (pid_t *) myReMap(info->csvChildPids, sizeof(pid_t) * TEMPSIZE, sizeof(pid_t) * *(info->numCsvPaths));
    info->csvPaths = (char **) myReMap(info->csvChildPids, sizeof(char *) * TEMPSIZE, sizeof(char *) * *(info->numCsvPaths));
    
    return 1;
}

// Finds all "propper" .csv files in the path <dirPath> and all subdirectories.
// <foundCsv>'s refrence will point to a newly allocated array of strings with
// paths to all "proper" .csv files found. <numFound>'s refrence will be set to
// the number of "proper" .csv files found. Returns 1 if the directory at <dirPath>
// is found, and 0 otherwise. To free, free all (*<csvPaths>)[i] (0 <= i < *<numFound>)
// and free *<csvPaths>.
int findCsvFiles(const char * dirPath, char * ** csvPaths, int * numFound) {

    (*csvPaths) = (char **) malloc(sizeof(char *) * TEMPSIZE);
    (*numFound) = 0;
    
    if (findCsvFilesHelper(dirPath, *csvPaths, numFound)) {
        
        (*csvPaths) = (char **) realloc(*csvPaths, sizeof(char *) * (*numFound));
        return 1;
        
    } else {
        
        free(*csvPaths);
        return 0;
    }
}

// This is a recursive helper function. Finds all "propper" .csv files in the
// path <dirPath> and all subdirectories thorugh recursive calling. <csvPaths>
// is a pre-allocated array of (char *)'s. <numFound>'s refrence keeps track
// of the current index of <csvPaths>. Returns 1 if the directory at <dirPath>
// is found, and 0 otherwise or if any of the recursive calls return a 0.
int findCsvFilesHelper(const char * dirPath, char ** csvPaths, int * numFound) {
    
    DIR * dir = opendir(dirPath);
    
    if (dir == NULL) {
        return 0;
    }
    
    char subDirPaths[255][TEMPSIZE];
    int sdp = 0; // Pointer for <subdirectories>.
    
    for (struct dirent * entry = readdir(dir); entry != NULL; entry = readdir(dir)) {
        
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            
            sprintf(subDirPaths[sdp], "%s/%s", dirPath, entry->d_name);
            sdp++;
            
        } else if (entry->d_type == DT_REG) {
            
            char csvPath[TEMPSIZE];
            sprintf(csvPath, "%s/%s", dirPath, entry->d_name);
            
            if (isProperCsv(csvPath)) {
                
                csvPaths[*numFound] = (char *) malloc((sizeof(char) * strlen(csvPath)) + 1);
                strcpy(csvPaths[*numFound], csvPath);
                (*numFound)++;
            }
        }
    }
    
    closedir(dir);
    
    for (int i = 0; i < sdp; i++) {
        
        if (!findCsvFilesHelper(subDirPaths[i], csvPaths, numFound)) {
            
            for (int i = 0; i < (*numFound); i++) {
                free(csvPaths[*numFound]);
            }
            
            return 0;
        }
    }
    
    return 1;
}

// ----------------------------
// | THIS IS YOUR JOB COASTA! |
// ----------------------------
// Returns 1 if <csvPath> is a path to a "proper" .csv file, else returns 0.
int isProperCsv(const char * csvPath) {
    return 1;
}

// ----------------------------
// | THIS IS YOUR JOB COASTA! |
// ----------------------------
// <path> is a path to a file or directory. This function should point
// <lineage>'s refrence to a newly allocated array of (char *), let's
// call it arr, such that arr[0] is the name of the file or directory
// pointed to by <path> and arr[n] where n > 0 is the parent directory
// of arr[n-1] for all n where 0 < n < return value. This function returns
// the number of elements in arr. To free, free (*<lineage>)[i] (0 <= i <
// returned value) and free *<lineage>.
unsigned int lineageParser(const char * path, char * ** lineage) {
    return 0;
}

// Creates <num> child processes of the parent. Returns a newly
// allocated array containing the pid of children to the parent.
// Returns NULL to all children. To free, free the returned
// pointer.
pid_t * multiFork(int num) {
    
    pid_t parent = getpid();
    pid_t children[num];
    
    for (int i = 0; i < num && getpid() == parent; i++) {
        children[i] = fork();
    }
    
    if (getpid() == parent) {
        
        pid_t * ret = malloc(sizeof(pid_t) * num);
        ret = memcpy(ret, children, sizeof(pid_t) * num);
        return ret;
    }
    
    return NULL;
}

// Returns a newly allocated copy of <source> of size <size> to all children.
// Returns NULL to the parent. <pipedFd> is a pre-piped file-descriptor,
// <isParent> is non-0 if the caller is the parent process, <numChildren> is
// the number of children. To free, free the returned pointer in all child
// processes.
void * pipeDataToChildren(const void * source, size_t size, int pipedFd[2],
                          int isParent, int numChildren) {
    
    if (isParent) {
        
        close(pipedFd[0]);
        
        for (int i = 0; i < numChildren; i++) {
            write(pipedFd[1], source, size);
        }
        
        close(pipedFd[1]);
        
        return NULL;
        
    } else {
        
        void * ret = malloc(size);
        
        close(pipedFd[1]);
        read(pipedFd[0], ret, size);
        close(pipedFd[0]);
        
        return ret;
    }
}

// Returns a newly allocated copy of <source> of size <size> to parent.
// Returns NULL to the child. <pipedFd> is a pre-piped file-descriptor,
// <isChild> is non-0 if the caller is the child process. To free, free
// the returned pointer in the parent process.
void * pipeDataToParent(const void * source, size_t size, int pipedFd[2], int isChild) {
    
    if (isChild) {
        
        close(pipedFd[0]);
        write(pipedFd[1], source, size);
        close(pipedFd[1]);
        
        return NULL;
        
    } else {
        
        void * ret = malloc(size);
        
        close(pipedFd[1]);
        read(pipedFd[0], ret, size);
        close(pipedFd[0]);
        
        return ret;
    }
}

// Allocates <size> bytes of memory shared between processes
// and returns a pointer to the allocated memory.
void * myMap(size_t size) {
    return mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
}

void * myReMap(void * address, size_t oldSize, size_t newSize) {
    return mremap(address, oldSize, newSize, 0);
}

