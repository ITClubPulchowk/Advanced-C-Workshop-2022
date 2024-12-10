#include <stdio.h>

void sum(int *a, int *b, int n) {
    for (int i = 0; i < n; i++) {
        a[i] += b[i];
    }
}

int main() {
    int a[] = {1, 2, 3, 4};
    int b[] = {3, 6, 1, 2};
    int c[4];
    sum(a, &a[1], 2);
}