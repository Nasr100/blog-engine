#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include "logger.h"
#include "../utils/file_handling.h"

#define MAX_LOG_LENGTH 256

 
 void get_time(char* time_str,size_t size){
	time_t now = time(NULL);
  	struct tm *t = localtime(&now);
  	strftime(time_str, size, "%d-%m-%Y %H:%M:%S", t);
}

logger_t* create_logger(const char* filename) {
	puts("CREATING LOGGER .....");
    logger_t* log = malloc(sizeof(logger_t));
    if(log != NULL){
		puts("MEMORY allocated suuccefully .....");
        log->log_lvl = LOG_INFO;
        log->fptr = NULL;
        if(filename){
			puts("log filename got it");
            strncpy(log->filename, filename, sizeof(log->filename)-1);
            log->fptr = fopen(filename, "a");
			if(log->fptr == NULL){
				puts("couldnt open log file");
				return NULL;
			}
        }
        return log;
    }
	puts("FAILED TO ALLOCATE MEMORY .....");
    return NULL;
}

char* get_loglvl(log_level_t lvl){
	switch(lvl){
		case 0 : {
			return "DEBUG";
			break;
		}
		case 1 : {
			return "INFO";
			break;
		}
		case 2 : {
			return "WARNING";
			break;
		}
		case 3 : {
			return "ERROR";
			break;
		}
		default: return "UNKNOWN";
	}
}
void set_logger_lvl(logger_t* log,log_level_t level){
	log->log_lvl = level;
}
void logger(logger_t* log,log_level_t lvl,char* format_str,va_list args){
	if(log->log_lvl > lvl) return; //if log level is bigger than the level skip it
	char loglvl[10]; 
	strcpy(loglvl,get_loglvl(lvl));
	char buffer[MAX_LOG_LENGTH];
    char timestamp[64];
    get_time(timestamp,sizeof(timestamp));
    
	vsnprintf(buffer,sizeof(buffer),format_str,args);//save the formatted str to a buffer
  
    
    char final_log[MAX_LOG_LENGTH + 128] ={0};
    snprintf(final_log,sizeof(final_log),"[%s] %s : %s\n",timestamp,loglvl,buffer);
    
    if(log->fptr != NULL){
    	fprintf(log->fptr, "%s", final_log);
    	fflush(log->fptr);  
	}else{
		printf("%s\n",final_log);
	}
    
    
}


void log_info(logger_t* log,char* str,...){
	
	
	va_list args; //list where arguments(...) gonna be stored
    va_start(args,str);   //start after str arg
   
	logger(log,LOG_INFO,str,args);
	va_end(args);   //clear the list
	
}

void log_debug(logger_t* log,char* str,...){
	
	
	va_list args; //list where arguments(...) gonna be stored
    va_start(args,str);   //start after str arg
   
	logger(log,LOG_DEBUG,str,args);
	va_end(args);   //clear the list
	
}

void log_warning(logger_t* log,char* str,...){

	
	va_list args; //list where arguments(...) gonna be stored
    va_start(args,str);   //start after str arg
   
	logger(log,LOG_WARNING,str,args);
	va_end(args);   //clear the list
	
}

void log_error(logger_t* log,char* str,...){

	va_list args; //list where arguments(...) gonna be stored
    va_start(args,str);   //start after str arg
   
	logger(log,LOG_ERROR,str,args);
	va_end(args);   //clear the list
	
}



void destroy_logger(logger_t* logger){
	if(logger == NULL)return;
		if(logger->fptr){
			fclose(logger->fptr);
		}
		logger->fptr = NULL;
		free(logger);
}
	

