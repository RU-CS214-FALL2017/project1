#define _GNU_SOURCE 1
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <string.h>

#include "forkTools.h"
#include "memTools.h"

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
    return address;
//    return mremap(address, oldSize, newSize, MREMAP_MAYMOVE);
}

// ---------------
// | INCOMPLETE! |
// ---------------
int unMapCsvDir(struct csvDir * dir) {
    return 0;
}

//void * initSharedMem(size_t size) {
//    
//    void * mem = myMap(size);
//    struct metadata * header = META_PTR_CAST mem;
//    struct metadata * footer = META_PTR_CAST ((CHAR_PTR_CAST mem) + size - META_SIZE);
//    
//    header->size = size;
//    header->dirty = 0;
//    memcpy(footer, header, META_SIZE);
//    
//    return mem;
//}
//
//void * myalloc(size_t size, void * mem, size_t memSize) {
//    
//    char * alloc = CHAR_PTR_CAST mem;
//    
//    while(alloc < (CHAR_PTR_CAST mem) + memSize) {
//        
//        if (!(META_PTR_CAST alloc)->dirty && (META_PTR_CAST alloc)->size <= size) {
//            
//            (META_PTR_CAST alloc)->dirty = 1;
//            (META_PTR_CAST alloc)->size = size;
//            struct metadata * footer = META_PTR_CAST (alloc + size + META_SIZE);
//            struct metadata * nextHeader = footer + 1;
//            struct metadata * nextFooter = META_PTR_CAST ((CHAR_PTR_CAST nextHeader))
//            
//        }
//        
//        alloc += ((META_PTR_CAST alloc)->size + (META_SIZE * 2));
//    }
//}

