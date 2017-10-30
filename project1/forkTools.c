#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>

#include "forkTools.h"
#include "tools.h"
#include "memTools.h"

// Sorts all CSV files by the column header <columnHeader> in the
// directory and subdirectories located at <path>. The sorted CSV
// file is stored in the directory of the path <outputDir>. <info>'s
// refrence will be set to point to newly mapped memory of info
// about the processed directory. To free, unmap all members of
// *(*<info>) and all members of subDirs within *(*<info>).
int processCsvDir(const char * path, struct csvDir * * info, const char * columnHeader, const char * outputDir) {
    
    DIR * dir = opendir(path);
    
    if (dir == NULL) {
        return 0;
    }
    
    (*info) = (struct csvDir *) myMap(sizeof(struct csvDir));
    
    (*info)->pid = myMap(sizeof(pid_t));
    *((*info)->pid) = getpid();
    (*info)->path = myMap(strlen(path));
    strcpy((*info)->path, path);
    
    (*info)->subChildPids = (pid_t *) myMap(sizeof(pid_t) * TEMPSIZE);
    (*info)->subDirs = (struct csvDir **) myMap(sizeof(struct csvDir *) * TEMPSIZE);
    (*info)->numSubDirs = myMap(sizeof(unsigned int));
    *((*info)->numSubDirs) = 0;
    
    (*info)->csvChildPids = (pid_t *) myMap(sizeof(pid_t) * TEMPSIZE);
    (*info)->csvPaths = (char **) myMap(sizeof(char *) * TEMPSIZE);
    (*info)->numCsvPaths = myMap(sizeof(unsigned int));
    *((*info)->numCsvPaths) = 0;
    
    for (struct dirent * entry = readdir(dir); entry != NULL; entry = readdir(dir)) {
        
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            
            pid_t child = fork();
            
            if (child == 0) {
                
                char subDirPath[TEMPSIZE];
                sprintf(subDirPath, "%s/%s", path, entry->d_name);
                
                processCsvDir(subDirPath, &(((*info)->subDirs)[*((*info)->numSubDirs)]), columnHeader, outputDir);
                
                exit(EXIT_SUCCESS);
            }
            
            ((*info)->subChildPids)[*((*info)->numSubDirs)] = child;
            (*((*info)->numSubDirs))++;
            printf("%d,", child);
            fflush(stdout);
            
        } else if (entry->d_type == DT_REG) {
            
            pid_t child = fork();
            
            if (child == 0) {
                
                char csvPath[TEMPSIZE];
                sprintf(csvPath, "%s/%s", path, entry->d_name);
                
                if (isProperCsv(csvPath)) {
                    
                    ((*info)->csvPaths)[*((*info)->numCsvPaths)] = myMap(strlen(csvPath) + 1);
                    strcpy(((*info)->csvPaths)[*((*info)->numCsvPaths)], csvPath);
                    
                    //                    if (outputDir == NULL) {
                    //                        sortCsv(csvPath, columnHeader, path);
                    //
                    //                    } else {
                    //                        sortCsv(csvPath, columnHeader, outputDir);
                    //                    }
                }
                
                exit(EXIT_SUCCESS);
            }
            
            ((*info)->csvChildPids)[*((*info)->numCsvPaths)] = child;
            (*((*info)->numCsvPaths))++;
            printf("%d,", child);
            fflush(stdout);
        }
    }
    
    (*info)->subChildPids = (pid_t *) myReMap((*info)->subChildPids, sizeof(pid_t) * TEMPSIZE, sizeof(pid_t) * *((*info)->numSubDirs));
    (*info)->subDirs = (struct csvDir **) myReMap((*info)->subDirs, sizeof(struct csvDir *) * TEMPSIZE, sizeof(struct csvDir *) * *((*info)->numSubDirs));
    
    (*info)->csvChildPids = (pid_t *) myReMap((*info)->csvChildPids, sizeof(pid_t) * TEMPSIZE, sizeof(pid_t) * *((*info)->numCsvPaths));
    (*info)->csvPaths = (char **) myReMap((*info)->csvPaths, sizeof(char *) * TEMPSIZE, sizeof(char *) * *((*info)->numCsvPaths));
    
    for (int i = 0; i < (*((*info)->numSubDirs) + *((*info)->numCsvPaths)); i++) {
        wait(NULL);
    }
    
    return 1;
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
