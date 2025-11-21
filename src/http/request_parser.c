#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h> 
#include <unistd.h>
#include <sys/socket.h>
#include "../include/request_parser.h"
#include "../../shared/utils/helpers.h"

void destroy_httpMessage(http_message* msg){
	if(msg==NULL)return;
	if(msg->body)free(msg->body);
	msg->body = NULL;
	if(msg->request)free(msg->request);
	msg->request = NULL;
	if(msg->headers_map){
		hashmap_t* map = msg->headers_map ;
			for(int i = 0; i < map->capacity; i++) {
			key_value_t* current = map->array[i];
			while(current != NULL) {
				key_value_t* next = current->next;
				if(current->value)free(current->value);
				free(current);
				current = next;
			}
		}
		free(msg->headers_map->array);
		free(msg->headers_map);
	}
	msg->headers_map=NULL;
	puts("HTTP MESSAGE OBJECT FREED SUCCEFFULLY!");
}


static void parse_header(char* header_line, hashmap_t* headers) {
    char key[256];
    char value[512];
    
    // Find the colon that separates key: value
    char* colon = strchr(header_line, ':');
    if (!colon) return;  // Invalid header format
    
    // Split into key and value
    *colon = '\0';  // Replace colon with null terminator
    
    // Key is everything before colon
    strncpy(key, header_line, sizeof(key) - 1);
    
    // Value is everything after colon (skip colon and any spaces)
    char* value_start = colon + 1;
    while (*value_start == ' ') value_start++;  // Skip leading spaces
    
    strncpy(value, value_start, sizeof(value) - 1);
    
    // Store in hashmap
    hashmap_insert_str(headers, key, value);
}

http_message* init_httpMessage(){
	http_request* request = malloc(sizeof(http_request));
	if(request==NULL)return NULL;
	http_message* httpMessage = malloc(sizeof(http_message));
	if(httpMessage==NULL){
		free(request);
		return NULL;
	}
	httpMessage->body = NULL;
	httpMessage->headers_map = NULL;
	httpMessage->request = request;
	return httpMessage;
}

http_message* parse_http_message(int socket,AppContext* ctx){
	if(ctx==NULL)return NULL;
	http_message* httpMessage = init_httpMessage();
	hashmap_t* headers_map = hashmap_create();
	if(headers_map==NULL){
		 destroy_httpMessage(httpMessage);
		 return NULL;
	}
	
	char* first_line = read_until(socket, "\r\n");
	sscanf(first_line, "%s %s %s", httpMessage->request->method,httpMessage->request->request_uri, httpMessage->request->http_version);


	while (1) {
		char* header_line = read_until(socket, "\r\n");
		if (strlen(header_line) == 0) break; 
		parse_header(header_line, headers_map);
	}

	httpMessage->headers_map = headers_map;

	char* cl = (char*)hashmap_get(headers_map,"Content-Length");
	if (cl!=NULL) {
		int content_length = atoi(cl);
		char* body_buffer = malloc(content_length + 1);
		read_exact_bytes(socket, body_buffer, content_length);
		httpMessage->body = body_buffer;
	}
	return httpMessage;
}





// void parse_json_body(char* body){
// 	char *post_data = strstr(body, "\r\n");
// 	cJSON* json;
//     if (post_data)
// 	{
// 	    post_data += 4; 
// 	    json = cJSON_Parse(post_data);
	    
	    
// 	    if (json == NULL) {
// 	        const char* error_ptr = cJSON_GetErrorPtr();
// 	        if (error_ptr != NULL) {
// 	            printf("JSON Error: %s\n", error_ptr);
// 	        }
// 	        log_error(server_logger,"Invalid JSON");
// 	        return;
//     	}
//     	printf("JSON parsed successfully!\n");
    
//     	cJSON* current_item = NULL;
//     	cJSON_ArrayForEach(current_item, json) {
//         printf("Key: %s, VALUE: %s\n", current_item->string, current_item->valuestring);
//     	}
//     }
// 	cJSON_Delete(json);       
// }













