#include <stdio.h>

struct test_struct {
    int a;
} __attribute__((mirrored));

__attribute__((mirrored)) void test_function(void) {
    int a;
    a += 1;
    return;
}

int main(void) {
    printf("Hi from example target program\n");

    return 0;
}
