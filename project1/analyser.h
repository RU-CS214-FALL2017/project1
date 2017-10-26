#ifndef analyser_h
#define analyser_h

// Used for representing an interesting fact.
typedef struct {
    
    char * header;
    char * value;
    char * relatives;
    
} FunFact;

void analize(char *** table, const unsigned int rows, const unsigned int columns);

#endif /* analyser_h */
