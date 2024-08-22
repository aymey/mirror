#include <stdio.h>

__attribute__((mirrored)) void test(void) {
    int a;
    a += 1;
    return;
}

int main(void) {
    printf("Hi from example target program\n");
    test();

    return 0;
}
