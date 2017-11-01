#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>

#include "forkTools.h"
#include "tools.h"
#include "memTools.h"
#include "sorter.h"

// Sorts all CSV files by the column header <columnHeader> in the
// directory and subdirectories located at <path>. The sorted CSV
// file is stored in the directory of the path <outputDir>. <info>'s
// refrence will be set to point to newly mapped memory of info
// about the processed directory. To free, unmap all members of
// *(*<info>) and all members of subDirs within *(*<info>).
int processCsvDir(const char * path, void * sharedMem, void * dirMem,
                  const char * columnHeaders, const char * outputDir) {
    
    DIR * dir = opendir(path);
    
    if (dir == NULL) {
        return 0;
    }
    
    struct csvDir * info = getDirFromPid(dirMem, getpid());
    info->pid = getpid();
    info->path = (char *) myalloc(strlen(path) + 1, sharedMem);
    strcpy(info->path, path);
    
    pid_t * tempDirPids = (pid_t *) myMap(sizeof(pid_t) * TEMPSIZE);
    info->numSubDirs = 0;
    
    struct csv * tempCsvs = (struct csv *) myMap(sizeof(struct csv) * TEMPSIZE);
    info->numCsvs = 0;
    
    for (struct dirent * entry = readdir(dir); entry != NULL; entry = readdir(dir)) {
        
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {

            pid_t child = fork();
            
            if (child == 0) {
                
                char subDirPath[TEMPSIZE];
                sprintf(subDirPath, "%s/%s", path, entry->d_name);
                processCsvDir(subDirPath, sharedMem, dirMem, columnHeaders, outputDir);
                
                exit(EXIT_SUCCESS);
            }

            tempDirPids[info->numSubDirs] = child;
            (info->numSubDirs)++;
            printf("%d,", child);
            fflush(stdout);
            
        } else if (entry->d_type == DT_REG) {
            
            char csvPath[TEMPSIZE];
            sprintf(csvPath, "%s/%s", path, entry->d_name);
            
            if (isProperCsv(csvPath)) {
                
                pid_t child = fork();
                
                if (child == 0) {
                    
                    if (outputDir == NULL) {
                        sortCsv(csvPath, columnHeaders, path);
                        
                    } else {
                        sortCsv(csvPath, columnHeaders, outputDir);
                    }
                    
                    exit(EXIT_SUCCESS);
                }
                
                tempCsvs[info->numCsvs].path = (char *) myalloc(strlen(csvPath) + 1, sharedMem);
                strcpy(tempCsvs[info->numCsvs].path, csvPath);
                tempCsvs[info->numCsvs].pid = child;
                
                (info->numCsvs)++;
                printf("%d,", child);
                fflush(stdout);
            }
        }
    }
    
    info->subDirsPids = (pid_t *) myalloc(sizeof(pid_t) * info->numSubDirs, sharedMem);
    memcpy(info->subDirsPids, tempDirPids, sizeof(pid_t) * info->numSubDirs);
    munmap(tempDirPids, sizeof(pid_t) * TEMPSIZE);
    
    info->csvs = (struct csv *) myalloc(sizeof(struct csv) * info->numCsvs, sharedMem);
    memcpy(info->csvs, tempCsvs, sizeof(struct csv) * info->numCsvs);
    munmap(tempCsvs, sizeof(struct csv) * TEMPSIZE);
    
    for (int i = 0; i < info->numSubDirs + info->numCsvs; i++) {
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
