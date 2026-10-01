#include <stdio.h>
#include <stdlib.h>  
#include <string.h>  
#include <netinet/in.h>
#include <sys/socket.h>
#include "../include/static_files.h"
#include "../include/response_builder.h"
#include "../include/request_parser.h"
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


 content_type_enum get_fullpath(char *uri, BlogConfig* config, char filepath[]) {
    if (!uri || !config || !filepath) {
        return UNKNOWN;
    }
    
    char* extension = get_afterLastDelimiter(uri, '.');
    printf("uri: '%s', Extension: '%s'\n", uri, extension ? extension : "NULL");
    
    content_type_enum type = UNKNOWN;
    
    // Handle files without extension - default to HTML
    if (!extension || strlen(extension) == 0) {
        snprintf(filepath, 4096, "%s/%s.html", config_get_html_path(config), uri);
        type = HTML;
    } else if(strcmp(extension, "js") == 0) {
        snprintf(filepath, 4096, "%s/%s", config_get_js_path(config), uri);
        type = JS;
    } else if(strcmp(extension, "css") == 0) {
        snprintf(filepath, 4096, "%s%s", config_get_css_path(config), uri);
        type = CSS;
    } else if(strcmp(extension, "jpg") == 0 || strcmp(extension, "jpeg") == 0 || 
              strcmp(extension, "png") == 0 || strcmp(extension, "gif") == 0) {
        snprintf(filepath, 4096, "%s/%s", config_get_imgs_path(config), uri);
        type = IMAGE;
    } else if(strcmp(extension, "html") == 0 || strcmp(extension, "htm") == 0) {
        snprintf(filepath, 4096, "%s/%s", config_get_html_path(config), uri);
        type = HTML;
    } else {
        // Unknown extension, try as HTML
        snprintf(filepath, 4096, "%s/%s.html", config_get_html_path(config), uri);
        type = HTML;
    }
    
    printf("Final filepath: %s\n", filepath);
    return type;
}


void render_404_page(int socket,AppContext* ctx){
    char path_404[1024];
   snprintf(path_404, sizeof(path_404), "%s/%s", 
            config_get_html_path(ctx->config), "404.html");
    int buffer_size;
    char* buffer =  read_file(path_404,buffer_size);
    send_response(404,HTML,buffer_size,socket,buffer);
}

char *get_home_header_html(const char *title)
{
    char *html = malloc(4096);

    if (!html)
        return NULL;

    snprintf(html, 4096,
        "<!DOCTYPE html>"
        "<html lang='en'>"
        "<head>"
        "<meta charset='UTF-8'>"
        "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
        "<link rel='stylesheet' href='home.css'>"
        "<title>%s</title>"
        "</head>"
        "<body>"
        "<header class='home-header'>"
        "<div class='blog-header'>"
        "<h1><a href=''>Benkhali Nasrallah</a></h1>"
        "<span class='header-span'>My journey through programming</span>"
        "</div>"
        "</header>",
        title
    );

    return html;
}

char* get_home_footer_html(){
   return 
   "<footer></footer>"
    "</html>";

}

void render_static_file(int socket,http_message* message,AppContext* ctx){
    puts("rendering file");
    char filepath[4096] = {0};
    size_t file_size;
    content_type_enum file_extension =  get_fullpath(message->request->request_uri,ctx->config,filepath);
    char* buffer = read_file(filepath,&file_size);
    if (buffer != NULL){
        send_response(200,file_extension,file_size,socket,buffer);
        }else{
            const char* error = "NOT FOUND";
            send_response(404,file_extension,strlen(error),socket,error);
                
            }
        
        free(buffer);


}