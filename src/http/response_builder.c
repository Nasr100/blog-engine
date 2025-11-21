#include<stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "../include/response_builder.h"
#include "../../shared/utils/file_utils.h"
#include <stdlib.h>                    // Add for free()
#include <string.h>                    // Add for strlen()
#include "../../shared/utils/helpers.h"
char* get_content_type(content_type_enum type) {
    switch(type){
        case IMAGE: return "image/jpeg";
        case HTML: return "text/html; charset=UTF-8";
        case CSS: return "text/css";
        case JS: return "text/javascript";
        case TEXT: return "text/plain";
        default: return "application/octet-stream";
    }
}

char* get_status_line(int code) {
    switch(code) {
        case 200: return "HTTP/1.1 200 OK";
        case 201: return "HTTP/1.1 201 OK";
        case 404: return "HTTP/1.1 404 Not Found";
        case 400: return "HTTP/1.1 400 Bad Request";
        default: return "HTTP/1.1 500 Internal Server Error";
    }
}

// void create_response(int code, content_type_enum content_type, int content_length, int socket) {
//     char* status_line = get_status_line(code);
//     char* content_type_str = get_content_type(content_type);
//     char response_buffer[8192];
//     memset(response_buffer,0,sizeof(response_buffer));
    
//     long unsigned int written = snprintf(response_buffer, sizeof(response_buffer),
//                          "%s\r\nContent-Type: %s\r\nContent-Length: %d\r\nConnection: close\r\n\r\n",
//                          status_line, content_type_str, content_length);
    
//     if (written > 0 && written < sizeof(response_buffer)) {
//         send(socket, response_buffer, written, 0);
//     }
    
// }

// void send_404_page( int socket){
// 	char* file = file_read("/mnt/c/Users/nasr0/OneDrive/Desktop/2026 C BOOTCAMP/BLOG ENGINE/public/template/html/404.html");
// 	create_response(404, HTML, strlen(file),socket);
// 	send(socket, file, strlen(file), 0);
// }


void send_json_response(int client_fd, const char* json_data, int status_code) {
    char response[4096];
    
    // Build HTTP response
    int length = snprintf(response, sizeof(response),
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        status_code, get_status_text(status_code),
        strlen(json_data),
        json_data);
    
    send(client_fd, response, length, 0);
}

void send_json_response_from_cjson(int client_fd, cJSON* json, int status_code) {
    char* json_str = cJSON_Print(json);
    send_json_response(client_fd, json_str, status_code);
    free(json_str);
}

// Helper to get status text
const char* get_status_text(int status_code) {
    switch(status_code) {
        case 200: return "OK";
        case 201: return "Created";
        case 400: return "Bad Request";
        case 404: return "Not Found";
        case 500: return "Internal Server Error";
        default: return "Unknown";
    }
}

void send_error_response(int client_fd, dynamic_array* error_messages, int status_code) {
    cJSON* error_json = cJSON_CreateObject();
    cJSON* errors_array = cJSON_CreateArray();
    
    // Convert each error message to JSON string in array
    for (size_t i = 0; i < error_messages->count; i++) {
        char* error_msg =(char*)error_messages->array[i];
        cJSON_AddItemToArray(errors_array, cJSON_CreateString(error_msg));
    }
    
    cJSON_AddItemToObject(error_json, "errors", errors_array);
    cJSON_AddNumberToObject(error_json, "status", status_code);
    
    send_json_response_from_cjson(client_fd, error_json, status_code);
    cJSON_Delete(error_json);
}


