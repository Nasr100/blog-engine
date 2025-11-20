#ifndef POSTSERVICE_H
#define POSTSERVICE_H
#include "../../dto/postDto.h"
#include "../../../shared/repository/post/postRepo.h"
#include "../../../shared/repository/tag/tagRepo.h"
typedef struct postService postService;

repsonseMsg* postService_add_post(postService* postService,postCreateDto* new_post);
repsonseMsg* postService_delete_post(postService* postService,int post_id);
repsonseMsg* postService_get_postById(postService* postService,int post_id);
repsonseMsg* postService_get_allPostsPreview(postService* postService);
postService* postService_create(postRepo* postRepo,tagRepo* tagRepo);
void postService_destroy(postService** ps);
#endif
