#include <stdio.h>
float function(float x) {
    return x + 2.0f;
}

typedef float (*Function_ptr)(float );

int main() {
    // equivalent to float (*ptr)(float )
    Function_ptr ptr = function; 

    printf("%f\n", ptr(2.0f));
}