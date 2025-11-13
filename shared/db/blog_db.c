#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include "blog_db.h"


void free_article_array(article_array* ar) {
    if(ar == NULL) return;
    int i = 0;
    for(i = 0; i < ar->count; i++) {
    	free(ar->articles[i]->body);
        free(ar->articles[i]);
    }
    free(ar->articles);
    free(ar);
}


article_array* create_article_array(){
	article_array* ar = malloc(sizeof(article_array));
	if(ar == NULL){
		puts("COULDNT ALLOCATE ARTICLE_ARRAY MEMORY");
		return NULL;
	}
	ar->count = 0;
	ar->size = 10;
	ar->articles = calloc(ar->size,sizeof(article*));
	if(ar->articles == NULL){
		puts("COULDNT ALLOCATE THE ARTICLES ARRAY ");
		return NULL;
	}
	return ar;
}


tag_array* create_tag_array(){
	tag_array* ta = malloc(sizeof(tag_array));
	if(ta == NULL){
		puts("COULDNT ALLOCATE TAG_ARRAY MEMORY");
		return NULL;
	}
	ta->count = 0;
	ta->size = 2;
	ta->tags = calloc(ta->size,sizeof(tag));
	if(ta->tags == NULL){
        puts("CCOULDNT ALLOCATE THE TAGS ARRAY ");
		return NULL;
	}
	return ta;
}




















