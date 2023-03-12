
// C program to illustrate
// pipe system call in C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MSGSIZE 18

char* hat = "I have a blue hat,";
char* coat = "and a yellow coat;";
char* umbrella = "bring an umbrella!";

int main() {
    char inbuf[MSGSIZE];
    int p[2], i;

    if (pipe(p) < 0) {
        perror("failed to open pipe");
        printf("here's what I meant to print:\n");
        printf("\t%s\n", hat);
        printf("\t%s\n", coat);
        printf("\t%s\n", umbrella);
        exit(1);
    }

    /* write pipe */
    int write_size = MSGSIZE; // try MSGSIZE-1 or +1?
    write(p[1], hat, write_size); 
    write(p[1], coat, write_size);
    write(p[1], umbrella, write_size);

    /* read pipe */
    for (i = 0; i < 3; i++) {
        read(p[0], inbuf, MSGSIZE); // try MSGSIZE-1 or +1?
        printf("%s\n", inbuf);
        sleep(1);
    }
    return 0;
}