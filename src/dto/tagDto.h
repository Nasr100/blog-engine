#ifndef TAGDTO_H
#define TAGDTO_H
#include "../../shared/repository/tag/tag.h"
typedef struct tagCreateDto{
	char name[MAX_TAG_LENGTH];
}tagCreateDto;

typedef struct tagResponseDto{
    int tag_id;
	char name[MAX_TAG_LENGTH];
    int is_deleted;
}tagResponseDto;

#endif