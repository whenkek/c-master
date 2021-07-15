#include <stdlib.h>
#include <stdio.h>

int main() {
    void *arr = malloc(sizeof(void*) * 2);
    int a = 1;
    arr = (void*) &a;
    a = 2;
    printf("%d\n", (*(int*) arr));
    return 0;
}