#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "reader.h"

Reader* reader_create(char path[]){
    if (path == NULL)
        return NULL;

    Reader *reader;
    reader = malloc(sizeof(*reader));

    if (reader == NULL)
        return NULL;
    
    if (access(path, F_OK) != 0)
        return NULL;


    *reader = (Reader){.file = fopen(path, "r"),
                       .path = path

    };
    return reader;
}

void reader_reopen(Reader* reader){
    if (access(reader->path, F_OK) != 0)
        return;
    
    fclose(reader->file);
    reader->file = fopen(reader->path, "r");
}



void reader_destroy(Reader* reader){
    if (reader == NULL)
        return;
    fclose(reader->file);
    free(reader);
}
