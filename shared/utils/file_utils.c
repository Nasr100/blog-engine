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
char* file_read_binary(const char* filename, size_t* file_size) {
    if (!filename) return NULL;
    
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Failed to open file: %s\n", filename);
        return NULL;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    *file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    if (*file_size == 0) {
        fclose(file);
        return NULL;
    }
    
    // Allocate memory
    char* buffer = malloc(*file_size + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }
    
    // Read file
    size_t bytes_read = fread(buffer, 1, *file_size, file);
    fclose(file);
    
    if (bytes_read != *file_size) {
        free(buffer);
        return NULL;
    }
    
    buffer[*file_size] = '\0'; // Null terminate for text files
    return buffer;
}
// char* file_get_basename(const char* path) {
//     char* path_copy = strdup(path);
//     char* base = strdup(basename(path_copy));
//     free(path_copy);
//     return base;
// }

