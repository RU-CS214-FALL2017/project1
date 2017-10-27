#ifndef free_h
#define free_h

#include <stdio.h>

void doubleFree(void ** alloc, int x);
void tripleFree(void *** alloc, int x, int y);

#endif /* free_h */
