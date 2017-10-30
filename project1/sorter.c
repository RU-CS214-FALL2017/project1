#include <string.h>
#include <stdlib.h>

#include "tools.h"
#include "sorter.h"
#include "memTools.h"

void merge(char *** table, const unsigned int columnIndex, const int areNumbers,
           const unsigned int start, const unsigned int mid,  const unsigned int end);

void cascadeSort(char *** table, unsigned int rows, unsigned int start,
                 const unsigned int end, const int * headers, const unsigned int numHeaders);

// Sorts a the CSV file at <csvPath> in ascending order
// on the column header <columnHeader>. Saves the sorted
// csv file in <outputDir>.
void sortCsv(const char * csvPath, const char * columnHeader, const char * outputDir) {
    
    FILE * csv = fopen(csvPath, "r");
    
    char *** table;
    unsigned int rows;
    unsigned int columns;
    
    fillTable(csv, &table, &rows, &columns);
    fclose(csv);
    
    sortByHeaders(csvPath, columnHeader, table, rows, columns);
        
    char * outputCsvPath = sortedCsvPath(csvPath, columnHeader, outputDir);
    FILE * sortedCsv = fopen(outputCsvPath, "w");
    free(outputCsvPath);
    
    printTable(sortedCsv, table, rows, columns);
    fclose(sortedCsv);
    
    tripleFree(table, rows, columns);
}

// Ascendingly sorts <table> with <rows> rows and <columns> columns according to
// the column with the first header in <columnHeaders>. Cascades the sort on the
// other headers in <columnHeaders> in order. <columnHeaders> should be a
// comma-delimited list of headers. Returns 1 if the first header in <columnHeaders>
// was found, else returns 0. Prints errors for cascaded headers not found.
void sortByHeaders(const char * csvPath, const char * columnHeaders, char *** table,
                      const unsigned int rows, const unsigned int columns) {
    
    char ** headers;
    unsigned int numHeaders = tokenizeRow(columnHeaders, &headers);
    
    int column = getColumnHeaderIndex(headers[0], table, columns);
    if (column == -1) {
        
        fprintf(stderr, "Specified column header, %s, not found in %s\n", headers[0], csvPath);
        exit(EXIT_FAILURE);
    }
    
    mergeSort(table, column, isNumericColumn(table, rows, column), 1, rows);
    
    int foundHeaders[numHeaders];
    foundHeaders[0] = column;
    int fhi = 1;
    
    for (int i = 1; i < numHeaders; i++) {
        
        foundHeaders[fhi] = getColumnHeaderIndex(headers[i], table, columns);
        
        if (foundHeaders[fhi] == -1) {
            
            fprintf(stderr, "Cannot cascade sort %s by specified column header, %s, because it was not found\n",
                    csvPath, headers[i]);
            
        } else {
            fhi++;
        }
    }
    
    cascadeSort(table, rows, 1, rows, foundHeaders, fhi);
}

// Cascades the sort of table with <rows> rows according to headers
// with indexes <headers>. <headers>[0] is the index of the header
// that <table> is sorted by from <start> to and not including <end>.
// <numHeaders is the number of elements in <headers>.
void cascadeSort(char *** table, unsigned int rows, unsigned int start,
                 const unsigned int end, const int * headers, const unsigned int numHeaders) {
    
    if (numHeaders > 1) {
        
        for (unsigned int j = start; j < end; j++) {
            
            if (strcmp(table[start][headers[0]], table[j][headers[0]])) {
                
                mergeSort(table, headers[1], isNumericColumn(table, rows, headers[1]), start, j);
                cascadeSort(table, rows, start, j, &headers[1], numHeaders - 1);
                start = j;
            }
        }
    }
}

// Ascendingly sorts <table> according to the column at index <columnIndex> from row at
// index <start> to row at index <end> - 1. If areNumbers is set to 0, the sort
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
