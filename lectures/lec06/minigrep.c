#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define MAX_LINE_LENGTH 80

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "minigrep: no pattern supplied\n");
        return 1;
    }

    char *filename = argv[1],
         *pattern = argv[2];

    char line[MAX_LINE_LENGTH] = {0};
    int line_count = 0;

    FILE *file = fopen(filename, "r");
    if (file == NULL)
        exit(EXIT_FAILURE);

    while (fgets(line, MAX_LINE_LENGTH, file)) {
        line_count++;

        for (int i = 0; i < strlen(line); i++) {
            int result = strcmp(line + i, pattern);  // try using strncmp instead
            // int result = strncmp(line + i, pattern, strlen(pattern));
            if (result == 0) {
                if (line[strlen(line) - 1] != '\n') printf("\n");
                printf("line[%03d:%d]: %s", line_count, i, line);
                break;
            }
        }
    }
    fclose(file);
    exit(EXIT_SUCCESS);

    return 0;
}