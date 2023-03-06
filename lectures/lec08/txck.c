#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

int main(int argc, char **argv) {
    int remaining = atoi(argv[1]);
    if (remaining <= 0) return 0;

    char remaining_str[10];
    sprintf(remaining_str, "%d", remaining-1);

    sleep(1);
    printf("%s\n", argv[0]);
    if (argv[0][1] == 'i') argv[0][1] = 'o';
    else argv[0][1] = 'i';

    execle(argv[0], argv[0], remaining_str, NULL, NULL);
    return -1;
}
