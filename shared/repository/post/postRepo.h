#ifndef POSTREPO_H

#define  POSTREPO_H
#include "post.h"
#include "../../db/database.h"
#include "../../utils/dynamic_array/dynamic_array.h"
#include "../../utils/hashmap/hashmap.h"

typedef struct postRepo{
    database* database;
}postRepo;

postRepo* create_post_repo(database* database);
post* insert_post(postRepo* postRepo,post* new_post);
dynamic_array* get_Posts_byTagId(postRepo* postRepo,int tag_id);
int remove_post(postRepo* postRepo,int post_id);
post* select_PostById(postRepo* postRepo,int post_id);
dynamic_array* select_allPosts_preview(postRepo* postRepo);
#endif