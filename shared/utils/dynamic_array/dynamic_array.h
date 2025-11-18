#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

typedef struct dynamic_array{
    int count;
    size_t size;
    void** array;
}dynamic_array;

dynamic_array* dynamic_array_create();
void dynamic_array_push_elem(dynamic_array* arr,void* elem);
void dynamic_array_resize(dynamic_array* arr);
void dynamic_array_push_str(dynamic_array* arr,const char* str);
void dynamic_array_resize(dynamic_array* arr);
void dynamic_array_destroy(dynamic_array* arr);
#endif