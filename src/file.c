#include "../incl/file.h"
#include "../incl/utils.h"
#include <stdio.h>
#include <stdlib.h>



File File_new(const char* filename)
{
    FILE* file_connection = fopen(filename, "r");
    File file = {0};

    if (!file_connection)
    {
        LOG_ERR("Cannot open file");
        perror(filename);
        return (File){NULL, 0};
    }

    fseek(file_connection, 0L, SEEK_END);
    file.len= ftell(file_connection);
    rewind(file_connection);

    file.text = malloc(sizeof(char) * file.len);

    if (!file.text)
    {
        LOG_ERR("Allocation for input file failed");
        fclose(file_connection);

        return (File){NULL, 0};
    }

    fread(file.text, sizeof(char), file.len, file_connection);

    fclose(file_connection);
    return file;
}



void File_delete(File* file)
{
    if (file) free(file->text);
    file->len = 0;
}
