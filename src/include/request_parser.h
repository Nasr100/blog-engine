#ifndef REQUEST_PARSER_H

#define REQUEST_PARSER_H
#include "../../shared/utils/hashmap/hashmap.h"
#include "app_context.h"
#include "server.h"
typedef struct http_request{
	char method[10];
	char request_uri[1024];
	char http_version[10];
}http_request;

typedef struct http_message{
	http_request* request;
	hashmap_t* headers_map;
	char* body; 
}http_message;

http_message* init_httpMessage();
http_message* parse_http_message(int socket,AppContext* ctx);
void destroy_httpMessage(http_message* msg);

#endif
