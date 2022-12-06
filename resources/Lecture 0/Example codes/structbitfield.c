#include <stdio.h>

struct Foo{
    int nibble:4;
    int septet:7;
    int octet:8;
    int pentet:5;
};
int main(){
    
    printf("%d\n",sizeof(struct Foo));
    return 0;
}