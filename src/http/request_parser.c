#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h> 
#include <unistd.h>
#include <sys/socket.h>
#include "../include/request_parser.h"

http_message* parse_http_message(int socket,logger_t* logger){
	
		//need free
	//reading socket data to a buffer
	char buffer[8192];
    int bytes_received = recv(socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received < 0) {
        return NULL;
    }
	buffer[bytes_received] = '\0';
//	printf("=======================BUFFER=================\n");
//	puts(buffer);
//	printf("=======================BUFFER=================\n");
	//parsing the data to a http_message struct
    char* copy_buffer = strdup(buffer); // make a copy for the buffer bc strtok is destructive
    char* delimiters = "\r\n\r\n";
    
    char* body = NULL;
   	http_request* request = (http_request*)malloc(sizeof(http_request));
    hashmap_t* map = hashmap_create();
    char key[1024], value[2048];
     
     //parsing the request-line 
    char* token = strtok(copy_buffer, delimiters);
    sscanf(token, "%s %s %s", request->method, request->request_uri, request->http_version);
    
    http_message* message = (http_message*)malloc(sizeof(http_message));
	//parsing the headers (fields) 
    while(1) {
    	token = strtok(NULL, delimiters);// take one line
    	if(token == NULL){
				break;
			}
    		sscanf(token,"%[^:]: %[^\n]",key,value); 
//    		printf("===============================\n");
//    		printf("token = %s\n",token);
//    		printf("key :%s\n",key);
//    		printf("value :%s\n",value);
//    		printf("===============================\n");
			hashmap_insert(map,key,value);
			if(strcasecmp(key,"Content-Length") == 0){
				
				int length = atoi(value)*8;
				message->body = malloc(sizeof(char) * length);
				if(message->body == NULL){
					log_info(logger,"CANT ALLOCATE MEMORY FOR MESSAGE BODY");
				}
				body = strstr(buffer, "\r\n\r\n");
				
				if(body){
					
					body += 4; 
					strcpy(message->body,body);
				}else{
					puts("body empty");
				}
    			break;
			}
    }
//    print_map(map);
    message->request = request;
    message->headers_map = map;
    free(copy_buffer); 
    return message;
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

void destroy_request(http_message* message,logger_t *logger){
	hashmap_destroy(message->headers_map);

	free(message);
	log_info(logger,"HTTP MESSAGE OBJECT FREED SUCCEFFULLY!");
}












