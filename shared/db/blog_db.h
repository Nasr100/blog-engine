#ifndef BLOG_DB_H

#define  BLOG_DB_H

#define MAX_TAG_LENGTH 20
#define MAX_TITLE_LENGTH 100
#define MAX_AUTHOR_LENGTH 100
#include "../libs/sqlite3.h"
typedef struct tag{
	int id;
	char tag[MAX_TAG_LENGTH];
}tag;

typedef struct article{
	char title[MAX_TITLE_LENGTH];
	char slug[MAX_TITLE_LENGTH];
	char author[MAX_AUTHOR_LENGTH];
	char* body;
	char created_date[100];
}article;


typedef struct article_tag{
	char article_id[MAX_TITLE_LENGTH];
	int tag_id;
}article_tag;

typedef struct article_array{
	article** articles;
	int size;
	int count;
}article_array;

typedef struct tag_array{
	tag** tags;
	int size;
	int count;
}tag_array;

typedef struct article_tag_array{
	article_tag** articles_tags;
	int size;
	int count;
}article_tag_array;

void free_article_array(article_array* ar);
article_array* create_article_array();
tag_array* create_tag_array();
#endif
