#include "file_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* file_read(char* filename) {
    FILE* fptr = fopen(filename, "r");
    if (fptr == NULL) {
        printf("Couldn't open %s\n", filename);
        return NULL;
    }
    
    fseek(fptr, 0, SEEK_END);
    long fsize = ftell(fptr);
    rewind(fptr);
    
    char* buffer = malloc(fsize + 1);
    memset(buffer, 0, fsize + 1);
    size_t bytes_read = fread(buffer, 1, fsize, fptr);
    buffer[bytes_read] = '\0';
    
    fclose(fptr);
    return buffer;
}

char* file_get_basename(const char* path) {
    char* path_copy = strdup(path);
    char* base = strdup(basename(path_copy));
    free(path_copy);
    return base;
}

