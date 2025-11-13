#include<stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "../include/response_builder.h"
#include "../../shared/utils/file_utils.h"
char* get_content_type(content_type_enum type) {
    switch(type) {
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
        case 404: return "HTTP/1.1 404 Not Found";
        case 400: return "HTTP/1.1 400 Bad Request";
        default: return "HTTP/1.1 500 Internal Server Error";
    }
}

void create_response(int code, content_type_enum content_type, int content_length, int socket) {
    char* status_line = get_status_line(code);
    char* content_type_str = get_content_type(content_type);
    char response_buffer[8192];
    memset(response_buffer,0,sizeof(response_buffer));
    
    long unsigned int written = snprintf(response_buffer, sizeof(response_buffer),
                         "%s\r\nContent-Type: %s\r\nContent-Length: %d\r\nConnection: close\r\n\r\n",
                         status_line, content_type_str, content_length);
    
    if (written > 0 && written < sizeof(response_buffer)) {
        send(socket, response_buffer, written, 0);
    }
    
}

void send_404_page( int socket){
	char* file = file_read("/mnt/c/Users/nasr0/OneDrive/Desktop/2026 C BOOTCAMP/BLOG ENGINE/public/template/html/404.html");
	create_response(404, HTML, strlen(file),socket);
	send(socket, file, strlen(file), 0);
}






