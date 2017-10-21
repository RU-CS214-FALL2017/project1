#include <string.h>

#include "tools.h"
#include "sorter.h"
#include "analyser.h"

char * mostFrequentInColumnSegment(char *** table, const unsigned int columnIndex,
                                   const unsigned int start, const unsigned int end);
void printFunFacts(FunFact * funFacts, int number, const char * phrase);
void saveFunFact(FunFact * funFacts, int * elements, char *** table, const unsigned int columnIndex,
                  const unsigned int relativeIndex, const unsigned int start, const unsigned int end);

// Prints interesting facts in <table> with <rows> rows and <columns> columns.
void analize(char *** table, const unsigned int rows, const unsigned int columns) {
    
    FunFact top[columns];
    char topRelatives[columns][4096];
    int topElms = 0;
    
    FunFact bottom[columns];
    char bottomRelatives[columns][4096];
    int bottomElms = 0;
    
    for (int i = 0; i < columns; i++) {
        
        top[i].relatives = topRelatives[i];
        bottom[i].relatives = bottomRelatives[i];
    }
    
    for(int i = 0;  i < columns; i++) {
        
        if(isNumericColumn(table, rows, i)) {
            
            mergeSort(table, i, 1, 1, rows);
            
            for (int j = 0; j < columns; j++) {
                
                if (!isNumericColumn(table, rows, j)) {
                    
                    saveFunFact(top, &topElms, table, j, i, rows - 10, rows);
                    saveFunFact(bottom, &bottomElms, table, j, i, 1, 10);
                }
            }
        }
    }
    
    printf("\n\n");
    printFunFacts(top, topElms, "greatest");
    printf("\n----------\n");
    printFunFacts(bottom, bottomElms, "least");
    printf("\n\n");
}

// Returns the most frequent value in column at <columnIndex> from row at index
// <start> inclusive to row at index <end> exclusive.
char * mostFrequentInColumnSegment(char *** table, const unsigned int columnIndex,
                                     const unsigned int start, const unsigned int end) {
    
    char * selected;
    int number;
    
    for (int i = start; i < end; i++) {
        
        selected = table[i][columnIndex];
        number = 0;
        
        for (int j = start; j < end; j++) {
            
            if (!strcmp(selected, table[j][columnIndex]) && strcmp(selected, "")) {
                number++;
            }
        }
        
        if (number > ((end - start) / 2)) {
            return selected;
        }
    }
    
    return NULL;
}

// Prints an the array <funFacts> with <number> elements, in a logical
// mannor with <phrase> given as extra information.
void printFunFacts(FunFact * funFacts, int number, const char * phrase) {
    
    if (number) {
        
        for (int i = 0; i < number; i++) {
            
            if (!strcmp("", funFacts[i].value)) {
                funFacts[i].value = "<blank>";
            }
            
            printf("\n%s %s appears often when the values are %s of %s\n", funFacts[i].header, funFacts[i].value, phrase, funFacts[i].relatives);
        }
        
    } else {
        printf("No interesting data found when values are %s", phrase);
    }
}

// Saves most frequent value in column at index <columnIndex> from row at
// index <start> to row at index <end> in <funFacts> at index <elements>
// when <table> is sorted according to the column at index <relativeIndex>
void saveFunFact(FunFact * funFacts, int * elements, char *** table, const unsigned int columnIndex,
                  const unsigned int relativeIndex, const unsigned int start, const unsigned int end) {
    
    char * frequent = mostFrequentInColumnSegment(table, columnIndex, start, end);
    
    if (frequent != NULL) {
        
        int found = 0;
        
        for (int k = 0; k < *elements; k++) {
            
            if(!strcmp(frequent, funFacts[k].value)) {
                
                strcat(funFacts[k].relatives, ", ");
                strcat(funFacts[k].relatives, table[0][relativeIndex]);
                
                found = 1;
            }
        }
        
        if (!found) {
            
            strcpy(funFacts[*elements].relatives, table[0][relativeIndex]);
            
            funFacts[*elements].header = table[0][columnIndex];
            funFacts[*elements].value = frequent;
            
            (*elements)++;
        }
    }
}
