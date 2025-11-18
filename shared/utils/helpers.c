#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"

char* my_strdup(char* str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str) + 1;
    char* copy = malloc(len);
    if (copy != NULL) {
        memcpy(copy, str, len);
    }
    return copy;
}


char* slug_converter(const char* title){
    size_t len = strlen(title) + 1;
    char* slug_output = malloc(len);
    int i = 0;
    for(i = 0;i<strlen(title);i++){
    	if(title[i] == ' '){
    		slug_output[i] = '-';
		}else if (isalnum(title[i])){
			slug_output[i] = tolower(title[i]);
		}
	}
    slug_output[i] = '\0';
    return slug_output;

}
