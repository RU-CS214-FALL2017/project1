#ifndef free_h
#define free_h

#include <stdio.h>

void doubleFree(char ** alloc, int x);
void tripleFree(char *** alloc, int x, int y);

#endif /* free_h */
