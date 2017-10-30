#define _GNU_SOURCE 1
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>

#include "forkTools.h"

// ---------------
// | INCOMPLETE! |
// ---------------
// Frees <alloc>[i] (0 <= i < <x>) and <alloc>.
void doubleFree(char ** alloc, int x) {
    
}

// ---------------
// | INCOMPLETE! |
// ---------------
// Frees <alloc>[i][j] (0 <= i < <x>, 0 <= j < <y>) and <alloc>.
void tripleFree(char *** alloc, int x, int y) {
    
}

// Allocates <size> bytes of memory shared between processes
// and returns a pointer to the allocated memory.
void * myMap(size_t size) {
    return mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
}

// Reallocates memory allocated with myMap. Returns NULL if
// <newSize is 0, otherwise returns the address to the
// reallocated memory.
void * myReMap(void * address, size_t oldSize, size_t newSize) {
    
    if (newSize == 0) {
        
        munmap(address, oldSize);
        return NULL;
    }
    
    return mremap(address, oldSize, newSize, MREMAP_MAYMOVE);
}

// ---------------
// | INCOMPLETE! |
// ---------------
int unMapCsvDir(struct csvDir * dir) {
    return 0;
}

