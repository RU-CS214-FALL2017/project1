#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>

#include "tools.h"
#include "forkTools.h"

int findCsvFilesHelper(const char * dirPath, char ** csvPaths, int * numFound);
void printDirTreeHelper(FILE * output, struct csvDir dir, unsigned int level);

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

// ---------------
// | INCOMPLETE! |
// ---------------
// Returns 1 if <csvPath> is a path to a "proper" .csv file, else returns 0.
int isProperCsv(const char * csvPath) {
    
    const char * extension = csvPath + strlen(csvPath) - 4;
    
    if (!strcmp(".csv", extension)) {
        return 1;
    }
    
    return 0;
}

// ---------------
// | INCOMPLETE! |
// ---------------
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

// ---------------
// | INCOMPLETE! |
// ---------------
// If the name of the CSV file located at <path> is A. This function returns
// the newly allocated string: "<outputDir>/A-sorted-<columnHeader>.csv". To
// free, free the returned pointer.
char * sortedCsvPath(const char * csvPath, const char * columnHeader, const char * outputDir) {
    
    char temp[strlen(csvPath) + 1];
    strcpy(temp, csvPath);
    
    char * token = strtok(temp, "/");
    char * tempTok = NULL;
    
    while (token != NULL) {
        
        tempTok = token;
        token = strtok(NULL, "/");
    }
    
    tempTok[strlen(tempTok) - 4] = '\0';
    
    char * ret = (char * ) malloc(strlen(outputDir) + strlen(tempTok) + strlen(columnHeader) + 9 + TEMPSIZE);
    sprintf(ret, "%s/%s-sorted-%s.csv", outputDir, tempTok, columnHeader);
    
    return ret;
}

// Returns the index of <columnHeader> in <table> with <columns>
// columns. If <columnHeader> not found, return -1.
int getColumnHeaderIndex(const char * columnHeader,
                         char *** table, const unsigned int columns) {
    
    for (int i = 0; i < columns; i++) {
        
        if (!strcmp(columnHeader, table[0][i])) {
            return i;
        }
    }
    
    return -1;
}

void printDirTree(FILE * output, struct csvDir dir) {
    printDirTreeHelper(output, dir, 0);
}

void printDirTreeHelper(FILE * output, struct csvDir dir, unsigned int level) {
    
    char * begin = "|   ";
    char * end = "| - ";
    
    for (int i = 0; i < level; i++){
        fprintf(output, "%s", begin);
    }
    
    fprintf(output, "%s%d: Processed the directory %s\n", end, dir.pid, dir.path);
    
    for (int i = 0; i < dir.numCsvs; i++) {
        
        for (int i = 0; i < (level + 1); i++){
            fprintf(output, "%s", begin);
        }
        
        fprintf(output, "%s%d: Sorted the file %s\n", end,
                dir.csvs[i].pid, dir.csvs[i].path);
    }
    
    for (int i = 0; i < dir.numSubDirs; i++) {
        printDirTreeHelper(output, dir.subDirs[i], level + 1);
    }
}



