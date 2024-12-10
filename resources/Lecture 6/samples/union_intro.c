#include <stdio.h>


typedef union TestUnion {
    int a; 
    float b;
    char name[30];
}TestUnion;

typedef struct TestStruct {
    int a; 
    float b;
    char name[30];
}TestStruct;


int main() {
#if 0
    TestUnion test;
    printf("(Union) Address of a: %p\n", &test.a);
    printf("(Union) Address of b: %p\n", &test.b);
    printf("(Union) Address of name: %p\n", &test.name);


    TestStruct test_struct;
    printf("(Struct) Address of a: %p\n", &test_struct.a);
    printf("(Struct) Address of b: %p\n", &test_struct.b);
    printf("(Struct) Address of name: %p\n", &test_struct.name);
#endif
// #if 0
    // Test the effect of different types of element of union
    TestUnion test;
    test.a = 2;
    test.b = 3.0;
    printf("Value of a: %d\n", test.a);
// #endif
}