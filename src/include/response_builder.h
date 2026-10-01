#ifndef RESPONSE_BUILDER_H
#include "../../shared/utils/dynamic_array/dynamic_array.h"
#include "../../shared/libs/cJSON.h"
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
	TEXT,
	JSON,
	UNKNOWN
}content_type_enum;



void send_error_response(int client_fd, dynamic_array* error_messages, int status_code);
ssize_t send_response(int code, content_type_enum content_type, size_t content_length,int socket,char* res_body);



#endif
