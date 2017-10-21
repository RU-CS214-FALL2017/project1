#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "tools.h"

unsigned int tokenizeRow(const char * line, char * ** row) {
    
    *row = (char **) malloc(strlen(line) * sizeof(char *));
    
    char tempChar = '\0';
    char tempCell[4096];
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

void removeChars (char * str, unsigned long startIndex, unsigned long endIndex) {
    
    unsigned long terminatingIndex = strlen(str);
    
    for(int i = 0; i <= (terminatingIndex - endIndex); i++) {
        
        str[startIndex + i] = str[endIndex + i];
    }
}

void fillTable(FILE * csvFile, char * *** table, unsigned int * rows, unsigned int * columns) {
    
    *table = (char ***) malloc(4194304 * sizeof(char **));
    char line[4096];
    *rows = 0;
    *columns = 0;
    
    while(fgets(line, 4096, csvFile) != NULL) {
        
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

void printTable (char *** table, unsigned int rows, unsigned int columns) {
    
    for (int i = 0; i < rows; i++) {
        
        if(strchr(table[i][0], ',') != NULL) {
            printf("\"%s\"", table[i][0]);
            
        } else {
            printf("%s", table[i][0]);
        }
        
        for (int j = 1; j < columns; j++) {
            
            if(strchr(table[i][j], ',') != NULL) {
                printf(",\"%s\"", table[i][j]);
                
            } else {
                printf(",%s", table[i][j]);
            }
        }
        
        printf("\n");
    }
}

int isNumber(const char * str) {
    
    for (int i = 0; i < strlen(str); i++) {
        
        if (!(isdigit(str[i]) || (str[i] == '.'))) {
            return 0;
        }
    }

    return 1;
}

int isXBeforeY (const char * x, const char * y, int areNumbers) {
    
    if (areNumbers) {
        return atof(x) <= atof(y);
        
    } else {
        return (strcmp(x, y) <= 0);
    }
}

int isNumericColumn(char *** table, int rows, int columnIndex) {
    
    for (int i = 1; i < rows; i++) {
        
        if (!isNumber(table[i][columnIndex])) {
            return 0;
        }
    }
    
    return 1;
}
