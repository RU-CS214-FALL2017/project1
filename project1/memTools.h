#ifndef memTools_h
#define memTools_h

#include <stdio.h>

void * myMap(size_t size);
void * myReMap(void * address, size_t oldSize, size_t newSize);
void doubleFree(char ** alloc, int x);
void tripleFree(char *** alloc, int x, int y);

#endif /* memTools_h */
