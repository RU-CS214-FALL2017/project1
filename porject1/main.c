#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "tools.h"
#include "sorter.h"
#include "analyser.h"
#include "mainTools.h"

int main() {
    
    int test = 5;
    
    
    pid_t * children = multiFork(test);
    
    if (children != NULL) {
        
        printf("children pids: \n");
        for (int i = 0; i < test; i++) {
            printf("%d\n", children[i]);
        }
        
        printf("waits: \n");
        for (int i = 0; i < test; i++) {
            printf("%d\n", wait(NULL));
        }
        
    }
    
    
    return 0;
}


//
//
//
//
//
//
//
//
//
//int main3(int argc, char ** argv) {
//
//    char * columnHeader = getColumnHeader(argc, argv);
//    char * inputDirecory = getInputDirectory(argc, argv);
//
//    if (inputDirecory == NULL) {
//        inputDirecory = ".";
//    }
//
//    char ** csvPaths;
//    int numCsv;
//
//    if (findCsvFiles(inputDirecory, &csvPaths, &numCsv)) {
//
//        pid_t children[numCsv];
//        pid_t pid = fork();
//        children[0] = pid;
//
//        for (int i = 1; i < numCsv && pid > 0; i++) {
//
//            pid = fork();
//            children[i] = pid;
//        }
//
//    } else {
//
//        fprintf(stderr, "Specified directory not found: %s\n", inputDirecory);
//        exit(EXIT_FAILURE);
//    }
//
//    exit(EXIT_SUCCESS);
//}
//
//int main2(int argc, char ** argv) {
//
//    for (int i = 0; i < argc; i++) { // Loops through arguments.
//
//        if (!strcmp("-c", argv[i])) { // Looks for column header with flag.
//
//            if ((i + 1) >= argc) { // Checks argument after flag.
//
//                perror("Column header not specified");
//                exit(EXIT_FAILURE);
//
//            } else {
//
//                char * inputDirPath = ".";
//                char * outputDirPath;
//                int odps; // Is output directory path specified.
//
//                for (int j = 0; j < argc; j++) { // Loops through arguments.
//
//                    if (!strcmp("-d", argv[j])) {  // Looks for directory path with flag.
//
//                        if ((j + 1) >= argc) { // Checks argument after flag.
//
//                            perror("Input directory path not specified with -d flag");
//                            exit(EXIT_FAILURE);
//
//                        } else {
//
//                        }
//                    }
//                }
//
//                char *** table;
//                unsigned int rows;
//                unsigned int columns;
//                const char * columnHeader = argv[i + 1];
//
//                fillTable(stdin, &table, &rows, &columns);
//
//                if (sortByHeader(columnHeader, table, rows, columns)) {
//                    printTable(table, rows, columns);
//
//                } else {
//
//                    fprintf(stderr, "Specified column header not found: %s", columnHeader);
//                    exit(EXIT_FAILURE);
//                }
//
//                freeTable(&table, rows, columns);
//            }
//        }
//
//        if (!strcmp("-i", argv[i])) {
//
//            char *** table;
//            unsigned int rows;
//            unsigned int columns;
//
//            fillTable(stdin, &table, &rows, &columns);
//
//            if (rows < 31) {
//
//                perror("Table is to small to find interesting data");
//                exit(EXIT_FAILURE);
//
//            } else {
//                analize(table, rows, columns);
//            }
//
//            freeTable(&table, rows, columns);
//        }
//    }
//
//    exit(EXIT_SUCCESS);
//}

