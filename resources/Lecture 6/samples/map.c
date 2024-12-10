#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>


#define ArraySize(arr) sizeof(arr) / sizeof(arr[0])

#if 0
#define lambda(return_type, function_body) \
  ({ \
    return_type anon_func_name_ function_body \
    anon_func_name_; \
  })
#endif



int* map(int *arr, uint8_t size, int (*func)(int)) {
    int *res = (int *)malloc(sizeof(int[size]));
    for (int i = 0; i < size; i++) {
        res[i] = func(arr[i]);
    }

    return res;
}

int multiply(int x) {
    return 2 * x;
}


int main() {
    int arr[] = {1, 2, 3, 4};
    int* res = map(arr, ArraySize(arr), multiply);

    for (int i = 0; i < 4; i++) {
        printf("%d\n", res[i]);   
    }
}