#ifndef memTools_h
#define memTools_h

#include "forkTools.h"

#define META_SIZE sizeof(struct metadata)
#define META_PTR_CAST (struct metadata *)
#define CHAR_PTR_CAST (char *)
#define VOID_PTR_CAST (void *)
#define PID_PTR_CAST (pid_t *)

// Represents metadata for my memory manager.
struct metadata {
    
    int64_t size;
    int64_t dirty;
};

void doubleFree(char ** alloc, int x);
void tripleFree(char *** alloc, int x, int y);
void * myMap(size_t size);
void * myReMap(void * address, size_t oldSize, size_t newSize);
int unMapCsvDir(struct csvDir * dir);
void * initSharedMem(pid_t initialPid);
void * getPidMem(void * sharedMem, pid_t pid);
struct csvDir * getDirFromPid(void * dirMem, pid_t pid);
void * initDirMem(void);
void * myalloc(size_t size, void * sharedMem);

#endif /* memTools_h */
