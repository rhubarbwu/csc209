#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char *name;

void sing(int sig) {
    printf("\n");
    printf("Happy birthday to you,\n");
    sleep(1);
    printf("Happy birthday to you,\n");
    sleep(1);
    printf("Happy birthday dear %s,\n", name);
    sleep(1);
    printf("Happy birthday to you!\n");
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: greeting name\n");
        exit(1);
    }
    name = argv[1];

    struct sigaction newact;

    // set signals to be blocked
    sigemptyset(&newact.sa_mask);
    sigaddset(&newact.sa_mask, SIGINT);

    newact.sa_flags = 0;       // default flags:
    newact.sa_handler = sing;  // custom handler

    // if (sigaction(SIGINT, &newact, NULL) == -1) exit(1);
    // if (sigaction(SIGTSTP, &newact, NULL) == -1) exit(1);

    // user-defined signal handler for SIGUSR1
    if (sigaction(SIGUSR1, &newact, NULL) == -1) {
        perror("sigacction");
        exit(1);
    };

    for (int i = 0; i < 10; i++) {  // Infinite loop
        printf(".");
        sleep(1);
    }
    return 0;
}