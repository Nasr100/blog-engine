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
        arr->size = arr->size * 2;
        void** new_array = realloc(arr->array,arr->size);
        arr->array = new_array;
}

void dynamic_array_push(dynamic_array* arr,void* elem){
    if(arr == NULL) return;
    if(arr->array == NULL){
        arr->array = malloc(arr->size*sizeof(void*));
    }
    if(arr->count >= arr->size){
        
    }
}

