#define _GNU_SOURCE 1
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <string.h>

#include "forkTools.h"
#include "memTools.h"

#define META_PTR_CAST (struct metadata *)
#define CHAR_PTR_CAST (char *)
#define VOID_PTR_CAST (void *)
#define PID_PTR_CAST (pid_t *)
#define DIR_PTR_CAST (struct csvDir *)

void * getMyMem(void * sharedMem);
void initMetadata(void * memSeg, size_t size);

// Frees <alloc>[i] (0 <= i < <x>) and <alloc>.
void doubleFree(char ** alloc, int x) {
    
    for (int i = 0; i < x; i++) {
        free(alloc[i]);
    }
    
    free(alloc);
}

// Frees <alloc>[i][j] (0 <= i < <x>, 0 <= j < <y>) and <alloc>.
void tripleFree(char *** alloc, int x, int y) {
    
    for (int i = 0; i < x; i++) {
        
        for (int j = 0; j < y; y++) {
            free(alloc[i][j]);
        }
        
        free(alloc[i]);
    }
    
    free(alloc);
}

// Allocates <size> bytes of memory shared between processes
// and returns a pointer to the allocated memory. To free,
// unmap the returned pionter with <size>.
void * myMap(size_t size) {
    return mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
}

// Initializes and returns shared memory that can be used
// with myalloc. To free, unmap the returned pointer with
// size SHARED_MEM_SIZE.
void * initSharedMem() {
    
    void * ret = myMap(SHARED_MEM_SIZE);
    *(PID_PTR_CAST ret) = getpid();
    
    char * seg = (CHAR_PTR_CAST ret) + sizeof(pid_t);
    
    while (seg < (CHAR_PTR_CAST ret) + (SHARED_MEM_SIZE - sizeof(pid_t))) {
        
        initMetadata(VOID_PTR_CAST seg, TEMPSIZE);
        seg += TEMPSIZE;
    }
    
    return ret;
}

// Initializes and returns memory to store struct csvDir,
// which can than be retrieved with getDirFromPid, To free,
// unmap the returned value of size DIR_MEM_SIZE.
void * initDirMem(){
    
    void * ret = myMap(DIR_MEM_SIZE);
    *(PID_PTR_CAST ret) = getpid();
    
    return ret;
}

// Retrieves the info saved by the process <pid> from <dirMem>
// of the dir.
struct csvDir * getDirFromPid(void * dirMem, pid_t pid) {
    
    pid_t index = pid - *(PID_PTR_CAST dirMem);
    char * ret = CHAR_PTR_CAST dirMem;
    ret += sizeof(pid_t) + (index * sizeof(struct csvDir));
    return (struct csvDir *) ret;
}

// Retrieves the segregated memory of the current pid
// in <sharedMem>.
void * getMyMem(void * sharedMem) {
    
    pid_t index = getpid() - *(PID_PTR_CAST sharedMem);
    char * ret = CHAR_PTR_CAST sharedMem;
    ret += sizeof(pid_t) + (index * TEMPSIZE);
    return VOID_PTR_CAST ret;
}

// Adds clean metadata to <memSeg> of <size>.
void initMetadata(void * memSeg, size_t size) {

    struct metadata * header = META_PTR_CAST memSeg;

    header->size = size;
    header->dirty = 0;
}

// Returns allocated memory of <size> from sharedMem of the current pid.
void * myalloc(size_t size, void * sharedMem) {

    void * mem = getMyMem(sharedMem);
    char * alloc = CHAR_PTR_CAST mem;

    while(alloc < (CHAR_PTR_CAST mem) + TEMPSIZE) {

        if (!(META_PTR_CAST alloc)->dirty && (META_PTR_CAST alloc)->size >= size) {

            int64_t oldSize = (META_PTR_CAST alloc)->size;
            
            (META_PTR_CAST alloc)->dirty = 1;
            (META_PTR_CAST alloc)->size = size;
            char * nextHeader = alloc + size + META_SIZE;
            initMetadata(VOID_PTR_CAST nextHeader, oldSize - size - META_SIZE);

            return (VOID_PTR_CAST alloc) + META_SIZE;
        }

        alloc += ((META_PTR_CAST alloc)->size + META_SIZE);
    }
    
    return NULL;
}

