#include <stdio.h>
#include <stdlib.h>  
#include <string.h>  
#include <netinet/in.h>
#include <sys/socket.h>
#include "../include/static_files.h"
#include "../include/response_builder.h"
#include "../../shared/config/config.h"
#include "../../shared/logger/logger.h"
#include "../../shared/utils/file_utils.h"
#include "../../shared/utils/helpers.h"


const char* get_content_type_string(content_type_enum type) {
    switch(type) {
        case HTML: return "text/html; charset=utf-8";
        case CSS: return "text/css; charset=utf-8";
        case JS: return "application/javascript; charset=utf-8";
        case IMAGE: return "image/jpeg"; // Adjust based on actual image type
        default: return "text/plain; charset=utf-8";
    }
}


static content_type_enum get_type_enum(char *filename, BlogConfig* config, char filepath[]) {
    if (!filename || !config || !filepath) {
        return UNKNOWN;
    }
    
    char* extension = get_afterLastDelimiter(filename, '.');
    printf("Filename: '%s', Extension: '%s'\n", filename, extension ? extension : "NULL");
    
    content_type_enum type = UNKNOWN;
    
    // Handle files without extension - default to HTML
    if (!extension || strlen(extension) == 0) {
        snprintf(filepath, 4096, "%s/%s.html", config_get_html_path(config), filename);
        type = HTML;
    } else if(strcmp(extension, "js") == 0) {
        snprintf(filepath, 4096, "%s/%s", config_get_js_path(config), filename);
        type = JS;
    } else if(strcmp(extension, "css") == 0) {
        snprintf(filepath, 4096, "%s/%s", config_get_css_path(config), filename);
        type = CSS;
    } else if(strcmp(extension, "jpg") == 0 || strcmp(extension, "jpeg") == 0 || 
              strcmp(extension, "png") == 0 || strcmp(extension, "gif") == 0) {
        snprintf(filepath, 4096, "%s/%s", config_get_imgs_path(config), filename);
        type = IMAGE;
    } else if(strcmp(extension, "html") == 0 || strcmp(extension, "htm") == 0) {
        snprintf(filepath, 4096, "%s/%s", config_get_html_path(config), filename);
        type = HTML;
    } else {
        // Unknown extension, try as HTML
        snprintf(filepath, 4096, "%s/%s.html", config_get_html_path(config), filename);
        type = HTML;
    }
    
    printf("Final filepath: %s\n", filepath);
    return type;
}

void send_404(int socket, AppContext* ctx) {
    char path_404[1024];
    snprintf(path_404, sizeof(path_404), "%s/%s", 
             config_get_html_path(ctx->config), "404.html");
    
    size_t file_size = 0;
    char* error_stream = file_read_binary(path_404, &file_size);
    
    if (error_stream && file_size > 0) {
        char response_header[1024];
        int header_len = snprintf(response_header, sizeof(response_header),
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %zu\r\n"
            "Connection: close\r\n"
            "\r\n", file_size);
        
        send(socket, response_header, header_len, 0);
        send(socket, error_stream, file_size, 0);
        free(error_stream);
        log_info(ctx->logger, "404 PAGE was sent to socket %d", socket);
    } else {
        // Fallback 404
        const char* fallback_404 = 
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 143\r\n"
            "Connection: close\r\n"
            "\r\n"
            "<html><body><h1>404 Not Found</h1><p>The requested resource was not found on this server.</p></body></html>";
        send(socket, fallback_404, strlen(fallback_404), 0);
        if (error_stream) free(error_stream);
    }
}

void render_static_file(int socket, char* uri, AppContext* ctx) {
    printf("render_static_file called\n");
    char filepath[4096] = {0};
    
    const char* filename = uri;
    if (uri && uri[0] == '/') {
        filename = uri + 1;
    }
    printf("Filename extracted: '%s'\n", filename ? filename : "NULL");
    
    if (!filename || strlen(filename) == 0) {
        send_404(socket, ctx);
        return;
    }
    
    content_type_enum type = get_type_enum((char*)filename, ctx->config, filepath);
    printf("Trying to open file: %s\n", filepath);
    
    // Use file_read_binary to handle all file types
    size_t file_size = 0;
    char* file_stream = file_read_binary(filepath, &file_size);
    
    if(file_stream == NULL || file_size == 0) {
        printf("Couldn't open %s\n", filepath);
        if (file_stream) free(file_stream);
        send_404(socket, ctx);
        return;
    }
    
    // BUILD PROPER HTTP RESPONSE
    char response_header[2048];
    const char* content_type = get_content_type_string(type);
    
    // Build headers
    int header_len = snprintf(response_header, sizeof(response_header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n", content_type, file_size);
    
    // Send headers
    send(socket, response_header, header_len, 0);
    
    // Send file content
    send(socket, file_stream, file_size, 0);
    
    free(file_stream);
    log_info(ctx->logger, "STATIC FILE SENT SUCCESSFULLY TO SOCKET: %d", socket);
}

void serve_static_file(int socket, const char* filename, AppContext* ctx) {
    printf("Serving static file: %s\n", filename);
    
    char filepath[4096] = {0};
    content_type_enum type = get_type_enum((char*)filename, ctx->config, filepath);
    
    printf("Looking for file at: %s\n", filepath);
    
    size_t file_size = 0;
    char* file_stream = file_read_binary(filepath, &file_size);
    
    if(file_stream == NULL || file_size == 0) {
        printf("Could not open file: %s\n", filepath);
        send_404(socket, ctx);
        if (file_stream) free(file_stream);
        return;
    }
    
    char response_header[2048];
    const char* content_type = get_content_type_string(type);
    
    int header_len = snprintf(response_header, sizeof(response_header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n", content_type, file_size);
    
    send(socket, response_header, header_len, 0);
    send(socket, file_stream, file_size, 0);
    
    free(file_stream);
    log_info(ctx->logger, "Static file '%s' sent to socket %d", filename, socket);
}