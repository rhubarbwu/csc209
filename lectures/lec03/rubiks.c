#include "stdio.h"

const int Y = 0, R = 1, B = 2, G = 3, O = 4, W = 5;

int main() {
    int rubiks_cube[6][3][3];  // 6 faces
    int rubiks_face[3][3] = {
        {Y, Y, R},
        {W, G, B},
        {Y, Y, R}};
    int k = 2, *p = rubiks_cube[k];
    for (; p < rubiks_face[k] + 2; p++) *p = O;

    int *row_ptr = rubiks_face[0];
    int *square_ptr = &row_ptr[2];  // pointing to rubiks_face[0][2]
    printf("%d\n", *square_ptr);
    square_ptr++;  // pointing to rubiks_face[1][0]
    printf("%d\n", *square_ptr);
    return 0;
}