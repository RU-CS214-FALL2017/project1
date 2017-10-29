#ifndef forkTools_h
#define forkTools_h

int processCsvDir(const char * path, struct csvDir * * info, const char * columnHeader, const char * outputDir);
pid_t * multiFork(int num);
void * pipeDataToChildren(const void * source, size_t size, int pipedFd[2], int isParent, int numChildren);
void * pipeDataToParent(const void * source, size_t size, int pipedFd[2], int isChild);

#endif /* forkTools_h */
