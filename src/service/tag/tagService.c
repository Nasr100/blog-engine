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
    printf("🚨 tagService_destroy called for: %p\n", (void*)*ts_ptr);
    if(ts_ptr == NULL || *ts_ptr == NULL) {
        printf("🚨 tagService_destroy: NULL pointer, returning early\n");
        return;
    }
    
    tagService* ts = *ts_ptr;
    
    printf("🔧 tagService_destroy - tagRepo: %p\n", (void*)ts->tagRepo);
    printf("🔧 tagService contents - tagRepo address: %p\n", (void*)ts->tagRepo);
    
    // DON'T free the tagRepo here - let app_context handle it
    // Just set to NULL
    ts->tagRepo = NULL;
    
    printf("🗑️  Freeing tagService struct at: %p\n", (void*)ts);
    free(ts);
    *ts_ptr = NULL;
    printf("✅ tagService_destroy completed\n");
}

