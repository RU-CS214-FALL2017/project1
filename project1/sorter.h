#ifndef sorter_h
#define sorter_h

int sortByHeader(const char * columnHeader, char *** table,
                 const unsigned int rows, const unsigned int columns);
void mergeSort(char *** table, const unsigned int columnIndex, const int areNumbers,
                 const unsigned int start, const unsigned int end);

#endif /* sorter_h */