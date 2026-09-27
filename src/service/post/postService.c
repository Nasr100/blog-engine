#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "postService.h"
#include "../../../shared/utils/helpers.h"
#include "../../../shared/db/database.h"



postService* postService_create(postRepo* postRepo,tagRepo* tagRepo){
    postService* ps = malloc(sizeof(postService));
    if(ps==NULL) return NULL;
    ps->postRepo = postRepo;
    ps->tagRepo = tagRepo;
    return ps;

}

void postService_destroy(postService** ps_ptr){
    if(ps_ptr == NULL || *ps_ptr == NULL) return;
    
    postService* ps = *ps_ptr;
    
    // JUST SET TO NULL - DON'T FREE THE REPOSITORIES!
    // They are shared with other services and managed by AppContext
    ps->postRepo = NULL;
    ps->tagRepo = NULL;
    
    free(ps);
    *ps_ptr = NULL;
}


repsonseMsg* postService_add_post(postService* postService,postCreateDto* new_post){
    if(postService == NULL) return NULL;
    dynamic_array* err_msgs = dynamic_array_create();
    char err[100];
    for(int i=0;i<new_post->count_tags;i++){
        tag* t = select_tagById(postService->tagRepo,new_post->tags_id[i]);
        if(t == NULL){
            sprintf(err,"tag with the id : %d not found",new_post->tags_id[i]);
            dynamic_array_push_str(err_msgs,err);
        }
    }
    if(err_msgs->count>0){
        return create_multiError_response("err",err_msgs,400);
    }
    post* postEntity = create_post(-1,new_post->title,new_post->slug,new_post->body,new_post->author,new_post->date,new_post->status,0,0,0,new_post->user_id,NULL,NULL);

    database_begin_transaction(postService->postRepo->database);
    post* p = insert_post(postService->postRepo,postEntity);
    if(p!=NULL){
        for(int i=0;i<new_post->count_tags;i++){
        tag* t = select_tagById(postService->tagRepo,new_post->tags_id[i]);
        dynamic_array_push_elem(p->tags,t);
    }
    database_commit(postService->postRepo->database);
        return create_success_response(p,201);
    }
    return create_error_response("err","post not added succefully",500);
}

repsonseMsg* postService_delete_post(postService* postService,int post_id){
    if(postService == NULL) return NULL;
    post* p = select_PostById(postService->postRepo,post_id);
    if(p!=NULL){
        if(remove_post(postService->postRepo,post_id) ==-1){
            return create_error_response("err","failed to delete post",500);
        }
        return create_success_response(p,200);
    }
    return create_error_response("err","post not found",400);
}

repsonseMsg* postService_get_postById(postService* postService,int post_id){
    if(postService == NULL) return NULL;
    post* p = select_PostById(postService->postRepo,post_id);
    if(p==NULL){
        return create_error_response("err","post not found",404);
    };
    p->tags = find_post_tags(postService->tagRepo,post_id);
    return create_success_response(p,200);
}

repsonseMsg* postService_get_allPostsPreview(postService* postService){
    printf("postService_get_allPostsPreview called\n");
    
    if(postService == NULL) {
        printf("postService is NULL\n");
        return create_error_response("err", "{\"err\":\"Service not initialized\"}", 500);
    }
    
    if(postService->postRepo == NULL) {
        printf("postRepo is NULL\n");
        return create_error_response("err", "{\"err\":\"Post repository not initialized\"}", 500);
    }
    
    printf("Calling select_allPosts_preview...\n");
    dynamic_array* posts = select_allPosts_preview(postService->postRepo);
    printf("select_allPosts_preview returned: %p\n", (void*)posts);
    
    if(posts == NULL){
        printf("select_allPosts_preview returned NULL (this should not happen!)\n");
        return create_error_response("err", "{\"err\":\"SERVER ERROR - Database error\"}", 500);
    }
    
    printf("Repository returned array with %d posts\n", posts->count);
    
    for(int i = 0; i < posts->count; i++){
        post* p = (post*)posts->array[i];
        printf("🔍 Getting tags for post %d...\n", p->post_id);
        p->tags = find_post_tags(postService->tagRepo, p->post_id);
        printf("✅ Post %d has tags: %p\n", p->post_id, (void*)p->tags);
    }
    
    return create_success_response(posts, 200);
}