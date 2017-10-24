#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mainTools.h"

// Returns the column header to sort by. If <argv> doesn't
// have a specified column header, the functions prints an
// error and exits with a failure of status.
char * getColumnHeader(int argc, char ** argv) {
    
    char * columnHeader = getFlagValue("-c", argc, argv);
    
    if (columnHeader == NULL) {
        
        perror("Column header not specified\nUse -h for help");
        exit(EXIT_FAILURE);
        
    } else {
        return columnHeader;
    }
}

// Returns the value of <flag> in <argv> with <argc> elements.
// Returns NULL if the flag value is not found.
char * getFlagValue(const char * flag, int argc, char ** argv) {
    
    int flagIndex = findFlag(flag, argc, argv);
    
    if (flagIndex == -1) {
        return NULL;
        
    } else {
        return getFlagValueHelper(flagIndex, argc, argv);
    }
}

// Returns the value of the flag at <flagIndex> of <argv> with
// <argc> elements. Returns NULL if the value is not found.
char * getFlagValueHelper(int flagIndex, int argc, char ** argv) {
    
    if ((flagIndex + 1) >= argc) {
        return NULL;
        
    } else {
        return argv[flagIndex + 1];
    }
}

// Returns the index of <flag> in <argv> with <argc> elements.
// Returns -1 if <flag> not found.
int findFlag(const char * flag, int argc, char ** argv) {
    
    for (int i = 0; i < argc; i++) {
        
        if (!strcmp(flag, argv[i])) {
            return i;
        }
    }
    
    return -1;
}

// Frees <table>'s refrence with <rows> rows and
// <columns> columns from the heap.
void freeTable(char * *** table, int rows, int columns) {
    
    for (int i = 0; i < rows; i++) {
        
        for (int j = 0; j < columns; j++) {
            
            free((*table)[i][j]);
        }
        
        free((*table)[i]);
    }
    
    free(*table);
}
