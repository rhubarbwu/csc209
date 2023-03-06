#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

int main(void) {
    printf("Mangoes\n");
    int r = fork();
    printf("Apples\n");
    if (r == 0) {
        printf("Oranges\n");
        if (fork() >= 0) printf("Bananas\n");
        return 0;
    }
    printf("Peaches\n");
    for (int i = 0; i < 3; i++) {
        if ((r = fork()) == 0) {
            printf("Pears\n");
            exit(0);
            printf("Nectarines\n");
            continue;
        }
        printf("Plums\n");
    }
    return 0;
}