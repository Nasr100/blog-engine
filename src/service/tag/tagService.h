#ifndef TAGSERVICE_H
#define TAGSERVICE_H
#include "../../../shared/utils/helpers.h"
#include "../../../shared/repository/tag/tagRepo.h"
#include "../../dto/tagDto.h"


typedef struct tagService tagService;
tagService* tagService_create(tagRepo* tagRepo);
repsonseMsg* tagService_add_tag(tagService* tagService,tagCreateDto* new_tag);
void tagService_destroy(tagService** ts_ptr);
#endif