#include <stdio.h>
#include <stdalign.h>

struct sse_t{
    alignas(16) float sse_data[4];
};

struct data{
    char x;
    alignas(128) char cacheline[128];
};

int main(){
    printf("sizeof(data) = %zu (1 byte + 127 byte padding + 128 byte array \n",sizeof(struct data));
    printf("alignment of sse_t is %zu\n",alignof(struct sse_t));
    return 0;
}