#ifndef memTools_h
#define memTools_h

#include <stdio.h>

void doubleFree(char ** alloc, int x);
void tripleFree(char *** alloc, int x, int y);
void * myMap(size_t size);
void * myReMap(void * address, size_t oldSize, size_t newSize);
int unMapCsvDir(struct csvDir * dir)

#endif /* memTools_h */
