#ifndef POSTCONTROLLER_H
#define POSTCONTROLLER_H
#include "../../include/app_context.h"

int postController_get_AllpostsPreview(int socket, http_message* message,AppContext* ctx);
int get_postbyId_json(int socket, http_message* message,AppContext* ctx);
int get_postbyId_html(int socket, http_message* message,AppContext* ctx);

#endif