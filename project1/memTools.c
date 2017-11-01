#define _GNU_SOURCE 1
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <string.h>

#include "forkTools.h"
#include "memTools.h"

void initMetadata(void * memSeg, size_t size);

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

void * initSharedMem(pid_t initialPid) {
    
    void * ret = myMap((TEMPSIZE * TEMPSIZE) + sizeof(pid_t));
    *(PID_PTR_CAST ret) = initialPid;
    
    char * seg = (CHAR_PTR_CAST ret) + sizeof(pid_t);
    
    while (seg < (CHAR_PTR_CAST ret) + (TEMPSIZE * TEMPSIZE)) {
        
        initMetadata(VOID_PTR_CAST seg, TEMPSIZE);
        seg += TEMPSIZE;
    }
    
    return ret;
}

void * initDirMem(){
    
    void * ret = myMap((TEMPSIZE * sizeof(struct csvDir)) + sizeof(pid_t));
    *(PID_PTR_CAST ret) = getpid();
    
    return ret;
}

struct csvDir * getDirFromPid(void * dirMem, pid_t pid) {
    
    pid_t index = pid - *(PID_PTR_CAST dirMem);
    char * ret = CHAR_PTR_CAST dirMem;
    ret += sizeof(pid_t) + (index * sizeof(struct csvDir));
    return (struct csvDir *) ret;
}

void * getPidMem(void * sharedMem, pid_t pid) {
    
    pid_t index = pid - *(PID_PTR_CAST sharedMem);
    char * ret = CHAR_PTR_CAST sharedMem;
    ret += sizeof(pid_t) + (index * TEMPSIZE);
    return VOID_PTR_CAST ret;
}

void initMetadata(void * memSeg, size_t size) {

    struct metadata * header = META_PTR_CAST memSeg;

    header->size = size;
    header->dirty = 0;
}

void * myalloc(size_t size, void * sharedMem) {

    void * mem = getPidMem(sharedMem, getpid());
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

