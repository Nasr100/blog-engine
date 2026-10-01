#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.h"
#include <ctype.h>
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


// repsonseMsg* create_success_response(void* data,int code_status){
//     repsonseMsg* res = malloc(sizeof(repsonseMsg));
//     if(res==NULL) return NULL;
//     res->data = data;
//     res->err_msg= NULL;
//     res->errMsgtype = NULL;
//     res->code_status = code_status;
//     return res;
// }

repsonseMsg* create_response(int success, char* message, int code_status,void* data){
    repsonseMsg* res = malloc(sizeof(repsonseMsg));
    if(res==NULL) return NULL;
    res->data = data;
    res->success= success;
    res->msg = my_strdup(message);
    res->code_status = code_status;
    return res;
}
repsonseMsg* create_multiError_response(int success, dynamic_array* messages, int code_status){
    repsonseMsg* res = malloc(sizeof(repsonseMsg));
    if(res==NULL) return NULL;
    res->data = NULL;
    res->msg= messages;
    res->success = success;
    res->code_status = code_status;
    return res;
}

void destroy_responseMsg(repsonseMsg* res){
    if(res == NULL)return ;
    free(res);
}

char* read_until(int socket, const char* delimiter) {
    char buffer[4096];
    int total_bytes = 0;
    int delim_len = strlen(delimiter);
    
    while (total_bytes < sizeof(buffer) - 1) {
        int bytes = recv(socket, buffer + total_bytes, 1, 0);
        if (bytes <= 0) return NULL;
        
        total_bytes++;
        buffer[total_bytes] = '\0';
        
        if (total_bytes >= delim_len && 
            strcmp(buffer + total_bytes - delim_len, delimiter) == 0) {
            buffer[total_bytes - delim_len] = '\0';
            return strdup(buffer);
        }
    }
    return NULL;
}

int read_exact_bytes(int socket, char* body_buffer, int content_length) {
    int total_read = 0;
    
    while (total_read < content_length) {
        int bytes = recv(socket, body_buffer + total_read, 
                        content_length - total_read, 0);
        if (bytes <= 0) return -1;  
        
        total_read += bytes;
    }
    
    body_buffer[content_length] = '\0';  
    return 0;  
}

int is_static(char* uri){
	return strstr(uri, ".css") || strstr(uri, ".js") || 
           strstr(uri, ".png") || strstr(uri, ".jpg") ||
           strcmp(uri, "/favicon.ico") == 0;
}

char* get_last_uri(char* path) {
    char* basename = strrchr(path, '/');
    return basename ? basename + 1 : path;
}

void pop_str(char** original_string) {
    if (original_string == NULL || *original_string == NULL) {
        return;
    }
    
    size_t len = strlen(*original_string);
    if (len == 0) {
        free(*original_string);
        *original_string = NULL;
        return;
    }
    
    char* new_string = malloc(len);  
    if (new_string == NULL) {
        return;  
    }
    
    strncpy(new_string, *original_string, len - 1);
    new_string[len - 1] = '\0';
    
    free(*original_string);
    *original_string = new_string;
    
}

int is_api(char* uri){
    char* temp = strtok(uri,"/");
    if(strcmp(temp,"api")) return 1;
    return 0; 
}

char* get_afterLastDelimiter(const char *filename, char delimiter) {
    const char *dot = strrchr(filename, delimiter); 
    if (!dot || dot == filename) { 
        return my_strdup("");  
    }
    return my_strdup(dot + 1);  
}