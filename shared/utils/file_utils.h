#ifndef FILE_UTILS_H
#define FILE_UTILS_H
#include "../logger/logger.h"
// char* file_read(char* filename); 
char* read_file( char* filename, size_t* file_size);
FILE* create_tmp_file();
#endif