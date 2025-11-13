#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

 typedef enum {
    LOG_DEBUG = 0, //0
    LOG_INFO = 1,//1
    LOG_WARNING = 2,//2
    LOG_ERROR = 3//3
} log_level_t;

typedef struct logger_t{
 	log_level_t log_lvl;
 	FILE* fptr;
    char filename[256];
 }logger_t;
 
 
logger_t* create_logger(const char* filename);
void set_logger_lvl(logger_t* log,log_level_t level);
void log_info(logger_t* log,char* str,...);
void log_error(logger_t* log,char* str,...);
void log_warning(logger_t* log,char* str,...);
void log_debug(logger_t* log,char* str,...);
char* get_loglvl(log_level_t lvl);
void destroy_logger(logger_t* logger);
#endif
