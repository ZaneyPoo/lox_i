#ifndef ZBASIC_FILE_H_
#define ZBASIC_FILE_H_
#include <stddef.h>


typedef struct File 
{
    char* text;
    size_t len;
} File;


File File_new(const char* filename);

// Name alludes to C++'s new and delete keywords. No files on disk are edited.
void File_delete(File* file);

#endif // ZBASIC_FILE_H_
