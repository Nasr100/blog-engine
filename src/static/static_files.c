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







static content_type_enum get_type_enum(char *filename,BlogConfig* config,char filepath[]){
	char* extension = get_afterLastDelimiter(filename,'.');
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
	}
	sprintf(filepath,"%s/%s.html",config_get_html_path(config),filename);
	return HTML;
	
}


void render_static_file(int socket,char* uri,AppContext* ctx){
	char filepath[4096] ;
	char* filename = get_afterLastDelimiter(uri,'/');
	content_type_enum type = get_type_enum(filename,ctx->config,filepath);
	char* file_stream = file_read(filepath);
	char resposne_line[100];
	if(file_stream == NULL){
		char path_404[1024];
		sprintf(path_404,"%s/%s",config_get_html_path(ctx->config),"404.html");
		file_stream = file_read(path_404);
		strcpy(resposne_line,get_status_line(404));
		send(socket,resposne_line,strlen(resposne_line),0);
		log_info(ctx->logger,"404 PAGE was sent to socket %d ",socket);
	}else{
		strcpy(resposne_line,get_status_line(200));
		send(socket,resposne_line,strlen(resposne_line),0);
		log_info(ctx->logger,"STATIC FILE SENT SUCCEFULLY TO SOCKET : %d ",socket);
	}
	send(socket,file_stream,strlen(file_stream),0);
	if(file_stream)free(file_stream);
	
	
	
}


