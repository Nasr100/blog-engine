#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tag.h"


tag* create_tag(int id,char *name,int is_deleted){
    if(name == NULL) return NULL;
    tag *t = malloc(sizeof(tag));
    if(t==NULL){
        return NULL;
    } 
    t->is_deleted = is_deleted;
    t->tag_id = id;
    strcpy(t->name,name);
    return t;
}


void destroy_tag(tag* tag){
    if(tag) free(tag);
}


