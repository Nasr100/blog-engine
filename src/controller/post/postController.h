#ifndef POSTCONTROLLER_H
#define POSTCONTROLLER_H
#include "../../include/app_context.h"

int postController_get_AllpostsPreview(int socket, http_message* message,AppContext* ctx);
int postController_get_postbyId(int socket, http_message* message,AppContext* ctx);

#endif