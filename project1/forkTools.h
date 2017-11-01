#ifndef forkTools_h
#define forkTools_h

#define TEMPSIZE 4096

// Represents a sorted CSV file.
struct csv {
    
    pid_t pid;
    char * path;
};

// Represents a processed directory.
struct csvDir {
    
    pid_t pid;
    char * path;
    pid_t * subDirsPids;
    struct csv * csvs;
    unsigned int numSubDirs;
    unsigned int numCsvs;
};

int processCsvDir(const char * path, void * sharedMem, void * dirMem,
                  const char * columnHeaders, const char * outputDir);
pid_t * multiFork(int num);
void * pipeDataToChildren(const void * source, size_t size, int pipedFd[2], int isParent, int numChildren);
void * pipeDataToParent(const void * source, size_t size, int pipedFd[2], int isChild);

#endif /* forkTools_h */
