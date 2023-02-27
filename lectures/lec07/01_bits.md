# Bit Manipulation

## Bitwise Operators: Basics

All data is composed of bits. Numerics and numerical macros/aliases can be manipulated with bitwise operators.

- Flip/negation (NOT): `~x`
- Union (inclusive-OR): `x|y`
- Intersection (AND): `x&y`
- Exlusive-OR (XOR): `x^y`

```c
unsigned x = 0;
printf("x = %u\n", x);
printf("~x = %u, %u\n", ~x, UINT_MAX);

unsigned y = 7;
printf("7|9 = %u\n", y | 9);
printf("7&9 = %u\n", y & 9);
printf("7^9 = %u\n", y ^ 9);
```

## Bitwise Operators: Shifts

All data is composed of bits. Numerics and numerical macros/aliases can be manipulated with bitwise operators.

- Bit-Shifts: `x<<y`, `x>>y`
  - Right bit-shift `>>` is logical on unsigned numbers.
  - Right bit-shift `>>` is arithmetic on signed numbers.
  - You can (cheaply) scale numbers to powers of 2 this way.

```c
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
```
