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
        case TEXT : return "text/plain";
        case JSON : return "application/json";
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
    
    // send_json_response_from_cjson(client_fd, error_json, status_code);
    cJSON_Delete(error_json);
}



ssize_t send_response(int code, content_type_enum content_type, size_t content_length,int socket,char* res_body){
    char* status_line = get_status_line(code);
    char* content_type_str = get_content_type(content_type);

    int header_length = snprintf(
        NULL,
        0,
        "%s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n",
        status_line,
        content_type_str,
        content_length
    );

    char header[header_length + 1];

    snprintf(
        header,
        sizeof(header),
        "%s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n",
        status_line,
        content_type_str,
        content_length
    );

    ssize_t sent = send(socket, header, header_length, 0);

    if (sent < 0)
        return sent;

    return send(socket, res_body, content_length, 0);
}



