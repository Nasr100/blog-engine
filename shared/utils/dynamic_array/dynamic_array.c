#include <stdio.h>
#include <stdlib.h>
#include "dynamic_array.h"


dynamic_array* dynamic_array_create(){
    dynamic_array* da = malloc(sizeof(dynamic_array));
    if(da == NULL){
        puts("Couldnt allocate memory for a dynamic array");
    }

    da->count = 0;
    da->size = 8;
    da->array = NULL;
}

void dynamic_array_resize(dynamic_array* arr){
        size_t new_size = arr->size * 2;
        void** new_array = realloc(arr->array, new_size * sizeof(void*));
        if (new_array == NULL) {
        puts("couldnt reallocate the dynamic array");
        return;
    }
        arr->array = new_array;
        arr->size = new_size;
}

void dynamic_array_push_elem(dynamic_array* arr,void* elem){
     
    if(arr == NULL) return;
    if(arr->array == NULL){
        arr->array = calloc(arr->size,sizeof(void*));
        if (arr->array == NULL) {
            puts("couldnt allocate memory for dynamic array");
            return;
        }
    }
    if(arr->count >= arr->size){
        dynamic_array_resize(arr);
    }
    arr->array[arr->count] = elem;
    arr->count++;
}

void dynamic_array_push_str(dynamic_array* arr, const char* str) {
    if (arr == NULL || str == NULL) return;
    if(arr->array == NULL){
        arr->array = calloc(arr->size,sizeof(void*));
        if (arr->array == NULL) {
            puts("couldnt allocate memory for dynamic array");
            return;
        }
    }
    if (arr->count >= arr->size) {
        dynamic_array_resize(arr);
    }
    
    arr->array[arr->count] = my_strdup(str); 
    arr->count++;
}


void dynamic_array_destroy(dynamic_array* arr) {
    if (arr == NULL) return;
    
    if (arr->array != NULL) {
        free(arr->array);
    }
    free(arr);
}


