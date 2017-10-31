#ifndef forkTools_h
#define forkTools_h

// Represents a processed directory.
struct csvDir {
    
    pid_t pid;
    char * path;
    
    struct csvDir * subDirs;
    unsigned int numSubDirs;
    
    struct csv * csvs;
    unsigned int numCsvs;
};

// Represents a sorted CSV file.
struct csv {
    
    pid_t pid;
    char * path;
};

int processCsvDir(const char * path, struct csvDir * info,
                  const char * columnHeaders, const char * outputDir);
pid_t * multiFork(int num);
void * pipeDataToChildren(const void * source, size_t size, int pipedFd[2], int isParent, int numChildren);
void * pipeDataToParent(const void * source, size_t size, int pipedFd[2], int isChild);

#endif /* forkTools_h */
