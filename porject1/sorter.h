#ifndef sorter_h
#define sorter_h

// Ascendingly sorts <table> with <rows> rows and <columns> columns according to
// the column with the header <columnHeader>. Returns 1 if <columnHeader>
// was found, else returns 0.
int sortByHeader(const char * columnHeader, char *** table,
                 const unsigned int rows, const unsigned int columns);

// Ascendingly sorts <table> according to the column at index <columnIndex> from row at
// index <start> to row at index <end>. If areNumbers is set to 0, the sort
// is done numerically otherwise it is done lexicographically.
void mergeSort(char *** table, const unsigned int columnIndex, const int areNumbers,
                 const unsigned int start, const unsigned int end);

#endif /* sorter_h */
