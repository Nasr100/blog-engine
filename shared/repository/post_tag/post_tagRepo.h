#ifndef POST_TAGREPO_H
#define  POST_TAGREPO_H
#include "../../db/database.h"
#include "../../utils/dynamic_array/dynamic_array.h"

typedef struct post_tagRepo{
    database* database;
}post_tagRepo;

post_tagRepo* create_postTag_repo(database* database);
int add_tagToPost(post_tagRepo* post_tagRepo,int post_id,int tag_id);
int delete_tagFromPost(post_tagRepo* post_tagRepo,int post_id,int tag_id);
dynamic_array* get_post_tags(post_tagRepo* post_tagRepo,int post_id);
#endif