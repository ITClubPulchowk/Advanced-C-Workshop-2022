#include <stdio.h>

//x64

struct Foo{
    char c;
    struct Foo *p;
    short x;

};

struct reFoo{  //only order changed
    struct Foo *p;
    char c;
    short x;
};

int main(){
    struct Foo foo;
    struct reFoo refoo;
    printf("%d\n",sizeof(foo));
    printf("%d\n",sizeof(refoo));
    return 0;
}