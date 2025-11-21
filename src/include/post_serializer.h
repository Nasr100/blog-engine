#ifndef POST_SERIALIZER_H
#define POST_SERIALIZER_H

#include "../../shared/libs/cJSON.h"
#include "../../shared/repository/post/post.h"
#include "../../shared/repository/tag/tag.h"
#include "../../shared/utils/dynamic_array/dynamic_array.h"

// Post to JSON
cJSON* post_to_json(post* post);
cJSON* post_to_json_basic(post* post); // Without user/tags for lists

// JSON to Post  
post* json_to_post(cJSON* json);

// Array conversions
cJSON* posts_to_json_array(dynamic_array* posts);
char* posts_to_json_string(dynamic_array* posts); // Direct string for responses

#endif