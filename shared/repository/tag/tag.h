#ifndef TAG_H

#define  TAG_H

#define MAX_TAG_LENGTH 20


typedef struct tag{
	int tag_id;
	char name[MAX_TAG_LENGTH];
    int is_deleted;
}tag;

tag* create_tag(int id,char *name,int is_deleted);
void destroy_tag(tag* tag);


#endif