# Multidimensional (N-D) Arrays

## Multidimensional (N-D) Arrays

Arrays can be multi-dimensional (N-D) to represent higher dimensional tensors.

```c
const int Y = 0, R = 1, B = 2, G = 3, O = 4, W = 5;
int rubiks_face[3][3] = {
    {Y, Y, R},
    {W, G, B},
    {Y, Y, R}};
```

The name of a two-dimensional array is a pointer to a pointer -- a double pointer. What's the type of the name of a three-dimensional array like `rubiks_cube`?

```c
int rubiks_cube[6][3][3]; // 6 faces, int ***
```

For any two-dimensional array `A`, the expression `A[k]` is a pointer to the first element in row `k` of the array.

```c
int k = 2, *p = rubiks_cube[k];
for (; p < rubiks_face[k] + 2; p++) *p = Y;
```

## Multidimensional Arrays: Row-Major Order

![row-major order of a 3x3 matrix](lec03/row-major.jpg){width=50%}

Although we visualize two-dimensional arrays as tables, that’s not the way they’re actually stored in computer memory. C stores arrays in _row-major order_, with row 0 first, then row 1, and so forth.

```c
int *row_ptr = rubiks_face[0];
int *square_ptr = &row_ptr[2]; // pointing to rubiks_face[0][2]
square_ptr++; // pointing to rubiks_face[1][0]
```

## Multidimensional Arrays: Go Big or Go Home

2/3-D arrays are common in modelling discrete surfaces/volumes. But in theory you can have any number of dimensions (memory permitting).

```c
int square[2][2], cube[3][3][3], hypercube7[7][7][7][7][7][7][7];
```

Thanks to pointers, you can navigate in many ways by indexing.

```c
// dimension-wise indexing
int hypercube7_mid = hypercube7[3][3][3][3][3][3][3];
// direct indexing
int cube_last = *(**cube + i*N*N + j*N + k);
for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) square[i][j] = i * 2 + j;
}
// arithmetic indexing
for (int p = 0; p < 4; p++) {
    printf("%d ", square[p / 2][p % 2]);
    if (p % 2 == 1) printf("\n");
}
```

## Multidimensional Arrays: Indexing & Traversal

How do you traverse down a column? Or along arbitrary dimension(s)? You can fix some of the indexing values by array or pointer arithmetic.

```c
// different dimensions, 3 rows, 4 columns
int rect[3][4];
for (int p = 0; p < 12; p++) rect[p / 4][p % 4] = p;

// first and last columns, and second row
for (int i = 0; i < 3; i++) printf("%d\n", rect[i][0]);
for (int i = 0; i < 3; i++) printf("%d\n", *(rect[i] + 3));
for (int j = 0; j < 4; j++) printf("%d ", (*rect[1] + j));
```

### Full code

[`github.com/rhubarbwu/csc209/blob/master/lectures/lec05/traversal.c`](https://github.com/rhubarbwu/csc209/blob/master/lectures/lec05/traversal.c)

### Traversing in an Arbitrary Order

Example: [`github.com/rhubarbwu/csc209/blob/master/lectures/lec05/3to3.c`](https://github.com/rhubarbwu/csc209/blob/master/lectures/lec05/3to3.c)
