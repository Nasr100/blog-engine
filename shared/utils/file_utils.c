#include "file_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




char* read_file(char* filename, size_t* file_size) {
    if (!filename) return NULL;
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Failed to open file: %s\n", filename);
        return NULL;
    }
    
    // Get file size
    puts("GETTING FILE SIZE");
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
    
    // if (bytes_read != *file_size) {
    //     free(buffer);
    //     return NULL;
    // }
    
    buffer[*file_size] = '\0'; // Null terminate for text files
    return buffer;
}

FILE* create_tmp_file(){
    FILE *file = tmpfile();
    return file;
}


