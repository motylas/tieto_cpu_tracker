#ifndef READER_H
#define READER_H

#include <stdio.h>

typedef struct Reader{
    FILE* file;
    char* path;
} Reader;
Reader* reader_create(char* path);
void reader_destroy(Reader* reader);
void reader_reopen(Reader* reader);

#endif
