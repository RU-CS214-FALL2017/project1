#include <string.h>
#include <stdlib.h>

#include "tools.h"
#include "sorter.h"

// Sorts a the CSV file at <csvPath> in ascending order
// on the column header <columnHeader>. Saves the sorted
// csv file in <outputDir>.
void sortCsv(const char * csvPath, const char * columnHeader, const char * outputDir) {
    
    FILE * csv = fopen(csvPath, "r");
    
    char *** table;
    unsigned int rows;
    unsigned int columns;
    
    fillTable(csv, &table, &rows, &columns);
    
    if (sortByHeader(columnHeader, table, rows, columns)) {
        
        char ** csvLineage;
        unsigned int lineageNum = lineageParser(csvPath, &csvLineage);
        char sortedCsvPath[strlen(csvLineage[0]) + strlen(outputDir) + 1];
        sprintf(sortedCsvPath, "%s/%s", outputDir, csvLineage[0]);
        
        
        FILE * sortedCsv = fopen(sortedCsvPath, "w");
        printTable(sortedCsv, table, rows, columns);
        
    } else {
        
        fprintf(stderr, "Specified column header, %s, not found in %s", columnHeader, csvPath);
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < rows; i++) {
        
        for (int j = 0; j < columns; j++) {
            
            free((table)[i][j]);
        }
        
        free((table)[i]);
    }
    
    free(table);
}

void merge(char *** table, const unsigned int columnIndex, const int areNumbers,
           const unsigned int start, const unsigned int mid,  const unsigned int end);

// Ascendingly sorts <table> with <rows> rows and <columns> columns according to
// the column with the header <columnHeader>. Returns 1 if <columnHeader>
// was found, else returns 0.
int sortByHeader(const char * columnHeader, char *** table,
                      const unsigned int rows, const unsigned int columns) {
    
    unsigned int columnIndex = 0;
    int found = 0;
    
    for (int i = 0; i < columns; i++) {
        
        if (!strcmp((table)[0][i], columnHeader)) {
            
            columnIndex = i;
            found = 1;
            break;
        }
    }
    
    if (found) {
        
        mergeSort(table, columnIndex, isNumericColumn(table, rows, columnIndex), 1, rows);
        return found;
        
    } else {
        return found;
    }
}

// Ascendingly sorts <table> according to the column at index <columnIndex> from row at
// index <start> to row at index <end>. If areNumbers is set to 0, the sort
// is done numerically otherwise it is done lexicographically.
void mergeSort(char *** table, const unsigned int columnIndex, const int areNumbers,
               const unsigned int start, const unsigned int end) {
    
    if ((end - start) > 1 ) {
        
        unsigned int mid = ((end - start) / 2) + start;
        
        mergeSort(table, columnIndex, areNumbers, start, mid);
        mergeSort(table, columnIndex, areNumbers, mid, end);
        
        merge(table, columnIndex, areNumbers, start, mid, end);
    }
}

// Merges sub rows from <table>, where subrows1 is from row at index <start>
// inclusive to row at index <mid> exclusive, and subrows2 is from row at
// index <mid> inclusive to row at index <end> exclusive. As the sub rows
// merge, they are sorted ascendingly according to the column at index
// <columnIndex>. If areNumbers is set to 0, the sort is done numerically
// otherwise it is done lexicographically.
void merge(char *** table, const unsigned int columnIndex, const int areNumbers,
           const unsigned int start, const unsigned int mid,  const unsigned int end) {
    
    char ** temp[end - start];
    unsigned int s = start;
    unsigned int m = mid;
    unsigned int i = 0;

    while(s < mid && m < end) {
        
        if(isXBeforeY((table)[s][columnIndex], (table)[m][columnIndex], areNumbers)) {
            
            temp[i] = (table)[s];
            s++;
            
        } else {
            
            temp[i] = (table)[m];
            m++;
        }
        
        i++;
    }
    
    while (s < mid) {
        
        temp[i] = (table)[s];
        s++;
        i++;
    }
    
    while (m < end) {
        
        temp[i] = (table)[m];
        m++;
        i++;
    }
    
    s = start;
    
    for (int j = 0; j < i; j++) {
        
        (table)[s] = temp[j];
        s++;
    }
}
