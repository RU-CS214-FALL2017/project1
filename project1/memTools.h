#ifndef memTools_h
#define memTools_h

#include "forkTools.h"

#define META_SIZE sizeof(struct metadata)
#define META_PTR_CAST (struct metadata *)
#define CHAR_PTR_CAST (char *)
#define VOID_PTR_CAST (void *)

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

#endif /* memTools_h */
