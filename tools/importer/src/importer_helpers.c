#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../../../shared/utils/helpers.h"
#include "include/importer_helpers.h"
#define MAX_TAGS 5


void split_tags_array(char* line,char* tags[]){
	char* copy_line = line;
	char* token = strtok(copy_line,":");
	token = strtok(NULL,":");

	char* copy_token = token;
	char* tag = strtok(copy_token+2,",");
	int tag_counter = 0;
	while(tag!=NULL && tag_counter<=MAX_TAGS){
		tags[tag_counter] = malloc(sizeof(char)*strlen(tag)+1);
		if(strrchr(tag,']') != NULL){
			strcpy(tags[tag_counter] ,tag);
			pop_str(&tags[tag_counter]);
		}else{
			strcpy(tags[tag_counter] ,tag);
		}
		tag = strtok(NULL,",");
		tag_counter++;
	}
}


void slug_converterv2(const char* title, char* slug_output){
    int i = 0;
    for(i = 0;i<strlen(title);i++){
    	if(title[i] == ' '){
    		slug_output[i] = '-';
		}else if (isalnum(title[i])){
			slug_output[i] = tolower(title[i]);
		}
	}
	
    slug_output[i] = '\0';

}


char* getValueV2(char* line) {
    char* colon_pos = strchr(line, ':');
    if (colon_pos == NULL) {
        return NULL;
    }
    
    char* value_start = colon_pos + 1;
    while (*value_start == ' ') {
        value_start++;
    }
    
    return value_start;
}
