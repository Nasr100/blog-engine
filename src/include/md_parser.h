#ifndef MD_PARSER_H

#define  MD_PARSER_H
#include "../../shared/libs/md4c.h"


typedef struct HtmlBuffer{
    char *data;
    size_t size;
} HtmlBuffer;

void append_html(const MD_CHAR *text, MD_SIZE size, void *userdata);



#endif
