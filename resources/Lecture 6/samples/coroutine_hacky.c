#include <stdio.h>
#include <inttypes.h>

uint8_t STATE = 0;
int simple_coroutine() {
    static int i;
    switch (STATE) {
        case 0:
            goto LABEL0;
            break;
        case 1:
            goto LABEL1;
            break;
        case 2:
            goto LABEL2;
            break;
    }
    STATE = 0;
    LABEL0:
    i = 0;
    for (; i < 10; i++) {
        STATE = 1;
        return i;
        LABEL1:
    }
    LABEL2:
    STATE = 2;
    return INT64_MAX;
}

int main() {
    for (int i = 0; i < 10; i++) {
        printf("%d\n", simple_coroutine());
    }
}