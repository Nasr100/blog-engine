#include "../../include/tag_serialization.h"
#include <stdlib.h>  // for malloc, free
#include <string.h>
cJSON* tag_to_json(tag* tag) {
    cJSON* json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "id", tag->tag_id);
    cJSON_AddStringToObject(json, "name", tag->name);
    cJSON_AddBoolToObject(json, "is_deleted", tag->is_deleted);
    return json;
}

cJSON* tags_to_json_array(dynamic_array* tags) {
    cJSON* array = cJSON_CreateArray();
    for (size_t i = 0; i < tags->count; i++) {
        tag* t = (tag*)tags->array[i];
        cJSON_AddItemToArray(array, tag_to_json(t));
    }
    return array;
}

char* tags_to_json_string(dynamic_array* tags) {
    cJSON* json_array = tags_to_json_array(tags);
    char* json_str = cJSON_Print(json_array);
    cJSON_Delete(json_array);
    return json_str;
}

tag* json_to_tag(cJSON* json) {
    tag* t = malloc(sizeof(tag));
    t->tag_id = cJSON_GetObjectItem(json, "id")->valueint;
    strncpy(t->name, cJSON_GetObjectItem(json, "name")->valuestring, MAX_TAG_LENGTH);
    t->is_deleted = cJSON_GetObjectItem(json, "is_deleted")->valueint;
    return t;
}

