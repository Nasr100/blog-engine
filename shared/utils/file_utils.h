#ifndef FILE_UTILS_H
#define FILE_UTILS_H
#include "../logger/logger.h"
char* file_read(char* filename);  // Your read_file
char* file_read_binary(const char* filename, size_t* file_size);
#endif