#ifndef STATIC_FILES_H

#define  STATIC_FILES_H

#include "../include/app_context.h"

void render_static_file(int socket,http_message* message,AppContext* ctx);
char* get_home_header_html(const char* title);
char* get_home_footer_html();
#endif
