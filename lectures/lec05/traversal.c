#include "stdio.h"

int main() {
    int square[3][3];

    // dimension-wise indexing
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) square[i][j] = i * 2 + j;
    }
    // arithmetic indexing
    for (int p = 0; p < 4; p++) {
        printf("%d ", square[p / 2][p % 2]);
        if (p % 2 == 1) printf("\n");
    }

    // different dimensions
    int rect[3][4];
    for (int p = 0; p < 12; p++) {
        rect[p / 4][p % 4] = p;
        printf("%d ", p);
        if (p % 4 == 3) printf("\n");
    }
    // last column
    for (int i = 0; i < 3; i++) printf("%d\n", rect[i][0]);
    printf("\n");
    for (int i = 0; i < 3; i++) printf("%d\n", *(rect[i] + 3));
    printf("\n");
    for (int j = 0; j < 4; j++) printf("%d ", (*rect[1] + j));
    printf("\n");

    return 0;
}