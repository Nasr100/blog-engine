#ifndef RESPONSE_BUILDER_H

#define  RESPONSE_BUILDER_H
// #include "../../shared/utils/hashmap/hashmap.h"
// typedef struct response{
// 	char* response_line;
// 	key_value kv;
// }response;


typedef enum content_type_enum{
	IMAGE,
	HTML,
	CSS,
	JS,
	TEXT
}content_type_enum;

void create_response(int code, content_type_enum content_type, int content_length, int socket) ;

void send_404_page( int socket);
#endif
