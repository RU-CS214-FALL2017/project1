#include <string.h>
#include <stdlib.h>

#include "tools.h"
#include "sorter.h"
#include "analyser.h"

void freeTable(char * *** table, int rows, int columns);

int main(int argc, const char * argv[]) {

    for (int i = 0; i < argc; i++) {

        if (!strcmp("-c", argv[i])) {

            if ((i + 1) == argc) {

                perror("Column header not specified");
                exit(EXIT_FAILURE);

            } else {

                char *** table;
                unsigned int rows;
                unsigned int columns;
                const char * columnHeader = argv[i + 1];
                
                fillTable(stdin, &table, &rows, &columns);
                
                if (sortByHeader(columnHeader, table, rows, columns)) {
                    printTable(table, rows, columns);
    
                } else {
                    
                    perror("Specified column header not found");
                    exit(EXIT_FAILURE);
                }
                
                freeTable(&table, rows, columns);
            }
        }
        
        if (!strcmp("-i", argv[i])) {
            
            char *** table;
            unsigned int rows;
            unsigned int columns;
            
            fillTable(stdin, &table, &rows, &columns);
            
            if (rows < 31) {
                
                perror("Table is to small to find interesting data");
                exit(EXIT_FAILURE);
                
            } else {
                analize(table, rows, columns);
            }
            
            freeTable(&table, rows, columns);
        }
    }

    exit(EXIT_SUCCESS);
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
