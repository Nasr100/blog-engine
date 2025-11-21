#ifndef HELPERS_H

#define  HELPERS_H

#include "dynamic_array/dynamic_array.h"

typedef struct repsonseMsg{
    void* data;
    char* errMsgtype;
    void* err_msg;
    int code_status;
}repsonseMsg;

repsonseMsg* create_success_response(void* data,int code_status);
repsonseMsg* create_error_response(char* msgType, char* messages, int code_status);
repsonseMsg* create_multiError_response(char* msgType, dynamic_array* messages, int code_status);
void destroy_responseMsg(repsonseMsg* res);


char* my_strdup(char* str);
char* slug_converter(const char* title);
char* read_until(int socket, const char* delimiter);
int read_exact_bytes(int socket, char* body_buffer, int content_length);
int is_static(char* uri);
char* get_last_uri(char* path) ;
void pop_str(char** original_string);
int is_api(char* uri);
char* get_afterLastDelimiter(const char *filename,char delimiter) ;
#endif