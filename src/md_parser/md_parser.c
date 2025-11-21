#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../shared/libs/md4c-html.h" 
#include "../include/md_parser.h"

 void append_html(const MD_CHAR *text, MD_SIZE size, void *userdata) {
    HtmlBuffer *buf = (HtmlBuffer *)userdata;

    char *new_data = realloc(buf->data, buf->size + size + 1);
    if (!new_data) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }

    buf->data = new_data;
    memcpy(buf->data + buf->size, text, size);
    buf->size += size;
    buf->data[buf->size] = '\0'; 
}

