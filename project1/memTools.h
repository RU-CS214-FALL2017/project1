#ifndef memTools_h
#define memTools_h

#include "forkTools.h"

#define META_SIZE sizeof(struct metadata)
#define DIR_MEM_SIZE ((TEMPSIZE * sizeof(struct csvDir)) + sizeof(pid_t))
#define SHARED_MEM_SIZE ((TEMPSIZE * TEMPSIZE) + sizeof(pid_t))

// Represents metadata for my memory manager.
struct metadata {
    
    int64_t size;
    int64_t dirty;
};

void doubleFree(char ** alloc, int x);
void tripleFree(char *** alloc, int x, int y);
void * myMap(size_t size);
void * initSharedMem(pid_t initialPid);
void * initDirMem(void);
struct csvDir * getDirFromPid(void * dirMem, pid_t pid);
void * myalloc(size_t size, void * sharedMem);

#endif /* memTools_h */
