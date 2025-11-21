#ifndef TAG_SERIALIZER_H
#define TAG_SERIALIZER_H
#include "../../shared/libs/cJSON.h"
#include "../../shared/repository/tag/tag.h"
#include "../../shared/utils/dynamic_array/dynamic_array.h"



cJSON* tag_to_json(tag* tag);
cJSON* tags_to_json_array(dynamic_array* tags);
char* tags_to_json_string(dynamic_array* tags);
tag* json_to_tag(cJSON* json);




#endif