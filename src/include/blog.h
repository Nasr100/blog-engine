#ifndef BLOG_H

#define  BLOG_H
#include "request_parser.h"
#include "../../shared/logger/logger.h"
#include "../include/app_context.h"
void render_article(int socket,http_message* message,AppContext* ctx);
void render_home(int socket,http_message* message,AppContext* ctx);


#endif
