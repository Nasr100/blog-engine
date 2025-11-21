#include <stdio.h>
#include <stdlib.h>
#include "tagService.h"

struct tagService{
    tagRepo* tagRepo;
};

tagService* tagService_create(tagRepo* tagRepo){
    tagService* ts = malloc(sizeof(tagService));
    if(ts==NULL)return NULL;
    ts->tagRepo = tagRepo;
    return ts;
}
repsonseMsg* tagService_add_tag(tagService* tagService,tagCreateDto* new_tag){
    if(tagService==NULL) return NULL;
    tag* t = select_tagByName(tagService->tagRepo,new_tag->name);
    if(t == NULL){
        tag* ntag = create_tag(-1,new_tag->name,0);
        ntag = insert_tag(tagService->tagRepo,ntag);
        return create_success_response(ntag,201);
    }
    destroy_tag(t);
    return create_error_response("err","tag alredy exists",400);
}

void tagService_destroy(tagService** ts_ptr){
    if (!ts_ptr || !*ts_ptr) return;
    tagService* ts = *ts_ptr;
    if(ts->tagRepo)free(ts->tagRepo);
    ts->tagRepo=NULL;
    free(ts);
    ts = NULL;
}


