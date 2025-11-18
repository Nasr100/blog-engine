#ifndef TAGREPO_H
#define TAGREPO_H
#include "tag.h"
#include "../../db/database.h"
#include "../../utils/dynamic_array/dynamic_array.h"


typedef struct tagRepo{
    database* database;
}tagRepo;

tagRepo* create_tagRepo(database* database);

tag* insert_tag(tagRepo* tagRepo,tag* new_tag);

tag* select_tagById(tagRepo* tagRepo,int tag_id);

tag* select_tagByName(tagRepo* tagRepo,char* name);

dynamic_array* select_allTags(tagRepo* tagRepo);

tag* update_tag(tagRepo* tagRepo,tag* new_tag);

int delete_tagById(tagRepo* tagRepo,int tag_id);

dynamic_array* find_post_tags(tagRepo* tagRepo,int post_id);
#endif