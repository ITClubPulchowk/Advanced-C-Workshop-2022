#include <stdio.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

void swap(void *src, void *dest, size_t elem_size) {
    char temp[elem_size];
    memcpy(temp, src, elem_size);
    memcpy(src, dest, elem_size);
    memcpy(dest, temp, elem_size);
}

void bubble_sort(void * arr, size_t data_size, size_t elem_size, int(*compare)(void * x, void * y)){
    size_t length = data_size / elem_size;
    for(int i = 0; i < length; i++){
        for(int j = 0; j < length - 1; j++){
            if(compare(arr + elem_size * j, arr + elem_size * (j + 1)) > 0){
                swap(arr + elem_size * j, (arr + elem_size * (j + 1)), elem_size); // basically swap(arr[j], arr[j + 1])
            };
        }

    }
}

int compare_int(void *a, void *b) {
    return *(int *)a < *(int *)b? 1: 0;
}

#define ArraySize(a) sizeof(a) / sizeof(a[0])

int main() {
    int arr[] = {4, 3, 2, 1, 5};
    bubble_sort(arr, sizeof(arr), sizeof(arr[0]), compare_int);
    for (int i = 0; i < ArraySize(arr); i++) {
        printf("%d\n", arr[i]);
    }
}