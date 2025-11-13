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



char filepath[4096] ;

static char* get_file_extension(const char *filename) {
    const char *dot = strrchr(filename, '.'); 
    if (!dot || dot == filename) { 
        return ""; 
    }
    return (char *)(dot + 1);
}

static content_type_enum get_type_enum(char *filename,BlogConfig* config){
	char* extension = get_file_extension(filename);
	puts(extension);
	if(strcmp(extension,"js") == 0){
		puts("JS FILE EXTENSION");
		sprintf(filepath,"%s/%s",config_get_js_path(config),filename);
		
		return JS;
	}else if(strcmp(extension,"css") == 0){
		puts("CSS FILE EXTENSION");
		sprintf(filepath,"%s/%s",config_get_css_path(config),filename);
		return CSS;
	}else if(strcmp(extension,"jpg")==0 || strcmp(extension,"jpeg")==0){
		sprintf(filepath,"%s/%s",config_get_imgs_path(config),filename);
		return IMAGE;
	}else if(strcmp(extension,"html")==0 || strcmp(extension,"htm")==0){
		return HTML;
	}
	return TEXT; 
}


void render_static_file(int socket,char* uri,AppContext* ctx){
	
	 char* filename= file_get_basename(uri);
	content_type_enum type = get_type_enum(filename,ctx->config);
	
	char* file_stream = file_read(filepath);
	if(file_stream == NULL){
		char message[4096];
		sprintf(message,"%s NOT FOUND",filename);
		create_response(404,TEXT,strlen(message),socket);
		send(socket,message,strlen(message),0);
		log_info(ctx->logger,"404 PAGE was sent to socket %d ",socket);
	}else{
		create_response(200,type,strlen(file_stream),socket);
		send(socket,file_stream,strlen(file_stream),0);
		log_info(ctx->logger,"STATIC FILE SENT SUCCEFULLY TO SOCKET : %d ",socket);
	}
	
	
	
}


