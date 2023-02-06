#include "stdio.h"

void init(int *tensor, int N) {
    int NN = 1;
    for (int p = 0; p < N; p++) NN *= N;
    for (int i = 0; i < NN; i++)
        *(tensor + i) = i;
}

void display_linear(int *tensor, int N) {
    printf("array of size %d**%d linear:\n", N, N);
    int NN = 1;
    for (int p = 0; p < N; p++) NN *= N;
    for (int i = 0; i < NN; i++)
        printf("%d ", i);
    printf("\n\n");
}

void display_rowcol_major(int tensor[3][3][3], int N) {
    printf("array of size %d**%d row-col-major:\n", N, N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                printf("%d ", tensor[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

void display_rowlay_major(int tensor[3][3][3], int N) {
    printf("array of size %d**%d col-row-major:\n", N, N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                printf("%d ", tensor[i][k][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

void display_collay_major(int tensor[3][3][3], int N) {
    printf("array of size %d**%d col-layer-major:\n", N, N);
    for (int j = 0; j < N; j++) {
        for (int k = 0; k < N; k++) {
            for (int i = 0; i < N; i++) {
                printf("%d ", tensor[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

void display_layrow_major(int tensor[3][3][3], int N) {
    printf("array of size %d**%d layer-row-major:\n", N, N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                printf("%d ", *(**tensor + k * N * N + i * N + j));
            }
            printf("\n");
        }
        printf("\n");
    }
}

int main() {
    const int N = 3;

    int stack_3to3[3][3][3] = {0};
    display_rowcol_major(stack_3to3, N);
    init(stack_3to3, N);
    display_linear(stack_3to3, N);
    display_rowcol_major(stack_3to3, N);
    display_rowlay_major(stack_3to3, N);
    display_collay_major(stack_3to3, N);
    display_layrow_major(stack_3to3, N);

    return 0;
}