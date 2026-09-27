#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h> 
#include <unistd.h>
#include <sys/socket.h>
#include "../include/request_parser.h"
#include "../../shared/utils/helpers.h"

void destroy_httpMessage(http_message* msg){
    if(msg == NULL) return;
    
    printf("Destroying http_message at: %p\n", (void*)msg);
    
    // Free body
    if(msg->body) {
        free(msg->body);
        msg->body = NULL;
    }
    
    // Destroy request object
    if(msg->request) {
        printf("Destroying HttpRequest at: %p\n", (void*)msg->request);
        
        // Free individual string fields
        if(msg->request->method) {
            free(msg->request->method);
            msg->request->method = NULL;
        }
        if(msg->request->request_uri) {
            free(msg->request->request_uri);
            msg->request->request_uri = NULL;
        }
        if(msg->request->http_version) {
            free(msg->request->http_version);
            msg->request->http_version = NULL;
        }
        
        // Free the request struct itself
        free(msg->request);
        msg->request = NULL;
    }
    
    // Destroy headers_map (ONLY ONCE)
    if(msg->headers_map){
        printf("Destroying headers_map at: %p\n", (void*)msg->headers_map);
        hashmap_destroy(msg->headers_map);  // Use your proper hashmap destructor
        msg->headers_map = NULL;
    }
    
    // Free the http_message struct itself
    free(msg);
    puts("HTTP MESSAGE OBJECT FREED SUCCESSFULLY!");
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
    if(request == NULL) return NULL;
    
    // CRITICAL: Initialize all fields to zero
    memset(request, 0, sizeof(http_request));
    
    http_message* httpMessage = malloc(sizeof(http_message));
    if(httpMessage == NULL){
        free(request);
        return NULL;
    }
    
    // Initialize all fields to zero
    memset(httpMessage, 0, sizeof(http_message));
    httpMessage->request = request;
    
    return httpMessage;
}

http_message* parse_http_message(int socket, AppContext* ctx){
    if(ctx == NULL) return NULL;
    
    http_message* httpMessage = init_httpMessage();
    if(httpMessage == NULL) return NULL;
    
    hashmap_t* headers_map = hashmap_create();
    if(headers_map == NULL){
        destroy_httpMessage(httpMessage);
        return NULL;
    }
    
    char* first_line = read_until(socket, "\r\n");
    if(!first_line) {
        destroy_httpMessage(httpMessage);
        hashmap_destroy(headers_map);
        return NULL;
    }
    
    // ALLOCATE memory for strings
    char method[32], uri[1024], version[32];
    sscanf(first_line, "%31s %1023s %31s", method, uri, version);
    
    // COPY strings into allocated memory
    httpMessage->request->method = my_strdup(method);
    httpMessage->request->request_uri = my_strdup(uri); 
    httpMessage->request->http_version = my_strdup(version);
    free(first_line);  // Don't forget to free this!
    
    // Parse headers
    while (1) {
        char* header_line = read_until(socket, "\r\n");
        if (!header_line || strlen(header_line) == 0) {
            if(header_line) free(header_line);
            break;
        }
        parse_header(header_line, headers_map);
        free(header_line);  // Free the line after parsing
    }
    
    httpMessage->headers_map = headers_map;
    
    // Parse body
    char* cl = (char*)hashmap_get(headers_map, "Content-Length");
    if (cl != NULL) {
        int content_length = atoi(cl);
        if(content_length > 0) {
            char* body_buffer = malloc(content_length + 1);
            if(body_buffer) {
                read_exact_bytes(socket, body_buffer, content_length);
                body_buffer[content_length] = '\0';
                httpMessage->body = body_buffer;
            }
        }
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













