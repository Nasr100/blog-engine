#include <stdio.h>
#include "../logger/logger.h"
#include "file_handling.h"
FILE* open_file(char* filename,char* mode){
	const char* fn = filename;
	FILE* fptr = fopen(fn,mode);
	if(fptr == NULL){
		return NULL;
	}
	return fptr;
}

void close_file(FILE* fptr){
	if(fptr!=NULL){
		 fclose(fptr);
	}
	
}
