#include <limits.h>
#include <stdio.h>

const int UPPER = 12;

int main() {
    unsigned x = 0;
    printf("x = %u\n", x);
    printf("~x = %u, %u\n", ~x, UINT_MAX);

    unsigned y = 7;
    printf("7|9 = %u\n", y | 9);
    printf("7&9 = %u\n", y & 9);
    printf("7^9 = %u\n", y ^ 9);

    printf("\n");
    for (unsigned z = 1; z < UPPER; z++) {
        printf("Computing powers of 2 scaled by %u...\n", z);
        for (unsigned i = 0; i < UPPER; i++)
            if (i == 11) printf("%u*(2^%u) = %u\n", z, i, z << i);
    }

    int s = -12815;
    printf("s>>2 = %d\n", s >> 2);          // s/4
    printf("s>>2>>3 = %d\n", s >> 2 >> 3);  // s/4/8

    unsigned u = 12815;
    printf("u>>1 = %u\n", u >> 1);          // u/2
    printf("u>>7>>3 = %u\n", u >> 7 >> 3);  // u/7/3

    return 0;
}