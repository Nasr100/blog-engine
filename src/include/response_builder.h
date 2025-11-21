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
	TEXT
}content_type_enum;

void create_response(int code, content_type_enum content_type, int content_length, int socket) ;

// void send_404_page( int socket);

void send_json_response(int client_fd, const char* json_data, int status_code);
void send_json_response_from_cjson(int client_fd, cJSON* json, int status_code);
void send_error_response(int client_fd, dynamic_array* error_messages, int status_code);
const char* get_status_text(int status_code);

#endif
