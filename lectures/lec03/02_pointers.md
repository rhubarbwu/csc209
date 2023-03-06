# Pointers

## Pointers: An Introduction

A _pointer_ wrapper of an address; operating on a pointer means operating on the address. A pointer variable can only point to objects of particular type, and such a variable must be declared with an asterisk \* preceded by the type. Pointer `ptr` points to object `obj` if the address of `obj` is stored in `ptr`.

```c
int obj = 7; int *ptr; // ptr starts as a NULL pointer
ptr = &obj;
int same_obj = *ptr;
```

Pointers can point to anything, including other pointers.

### Operators:

- addressing (`&`): `&obj` takes the address of `obj`, to store in `ptr`.
- indirection (`*`): `*ptr` dereferences the `ptr` to get `obj`.
  - Dereferencing a `NULL` pointer results in errors.

## Pointers: Operators

They're formatted into strings with `%p`.

```c
#include <stdio.h>
int main() {
    char ch = 'Y'; char *ch_pt = &ch;
    printf("ch_pt points to %c\n", *ch_pt);

    int i = 5;
    printf("Value and address of i: %d, %p\n", i, &i);
    int *pt = &i;
    printf("Value and address of pt: %p, %p\n", pt, &pt);
    printf("Value pointed to by pt: %d\n", *pt);

    return 0;
}
```

[Source: PCRS (University of Toronto)](https://c-programming.onlinelearning.utoronto.ca/webdocs/2_pointers_memory/b_pointers/RESOURCES/pointers_intro_int.c)

## Pointers: Operators

Dereferenced values can be operated on, or incremented/decremented.

```c
#include <stdio.h>
int main() {
    int i = 7, j = i;
    int *pt = &i;
    *pt = 9;
    printf("Value of i: %d\n", i); // 9
    printf("Value of j: %d\n", j); // 7
    printf("Value of j | (i+1): %d\n", j ^ *pt++); // 14
    printf("pt points to %d\n", ++*pt); // 8
    return 0;
}
```

[Source: PCRS (University of Toronto)](https://c-programming.onlinelearning.utoronto.ca/webdocs/2_pointers_memory/b_pointers/RESOURCES/pointers_intro_int.c)

## Pointers: Why Pointers?

- Easy access to and abstraction of complex structures.
- Allows reference to the same data when desired.
- Pointers consume less memory than deep copies.
- Convenient null values for initialization/error-checking.

## Pointers: Function Arguments by Value

C passes arguments by value. Implicit casting is performed on numerical function arguments; beware of truncation!

```c
#include "math.h"
#include "stdio.h"
int as_long(long l) { return l; }
float as_float(float d) { return d; }

int main() {
    int nine_plus_ten = 21;
    long massive = __LONG_MAX__ - nine_plus_ten;
    printf("%ld -> %d\n", massive, as_int(massive));
    double pi = M_PI;  // approximate the approximation
    printf("%1.32f -> %1.32f\n", pi / 2, as_float(pi) / 2);
    return 0;
}
```

Full code: [`raw.githubusercontent.com/rhubarbwu/csc209/master/lectures/lec04/arg_cast.c`](https://raw.githubusercontent.com/rhubarbwu/csc209/master/lectures/lec04/arg_cast.c)

## Pointers: Function Arguments by Value

What does this do to `mass`? It's being passed by value.

```c
#include <stdio.h>
#define half_life 12
#define time 100

void decay(double mass) {
    mass /= 2;
}
int main() {
    double mass = 244817;
    for (int i = 1; i < time; i++)
        if (i % half_life == 1)
            decay(mass);
    printf("After %d, %lf remains.\n", time, mass);
    return 0;
}
```

## Pointers: Why Pointers?

- Easy access to and abstraction of complex structures.
- Allows reference to the same data when desired.
- Pointers consume less memory than deep copies.
- Convenient null values for initialization/error-checking.

### As Function Arguments

Pointers allow you to pass primitives or structures by reference, rather than value. Instead of copying and passing the entire structure, copy/pass the pointer(s) in constant time.

## Pointers: Function Arguments by Reference

What about this? It's passed by reference.

```c
#include <stdio.h>
#define half_life 12
#define time 100

void approx_decay(double *mass_ptr) {
    *mass_ptr /= 1.4142857;
}
int main() {
    double mass = 244817;
    for (int i = 1; i < time; i++)
        if (i % (half_life / 2) == 1)
            approx_decay(&mass);
    printf("After %d, %lf remains.\n", time, mass);
    return 0;
}
```

## Pointers: Pointers to Pointers (to Pointers ...)

Because pointers can point to anything, you also have pointers to pointers.

```c
int main() {
    int i = 81; int *pt = &i; int **pt_ptr = &pt;

    int *r= *pt_ptr; // intermediate dereference
    int k = *r; // complete the dereference

    int k1 = **pt_ptr; // direct double dereference

    int ***pt_ptr_ptr = &pt_ptr; // triple pointer
    int k2 = ***pt_ptr_ptr;
    return 0;
}
```

[Source: PCRS (University of Toronto)](https://c-programming.onlinelearning.utoronto.ca/webdocs/2_pointers_memory/b_pointers/RESOURCES/pointers_intro_int.c)

## Pointers: Relationship with Arrays

The relationship between pointers and arrays in C is a close one. Understanding this relationship is critical for mastering C.

- C allows to perform addition and subtraction on pointers to array elements. This leads to an alternative way of processing arrays in which pointers take the place of array subscripts.
- Pointers can point to array elements. Here's an example:

  ```c
  int a[10], *p;
  p = &a[0];
  *p = 5; // stores 4 in a[0]
  ```

- A pointer is not an array but it can contain the address of an array. An array is not a pointer either but the compiler interprets the name of an array as the address of its 0th element.

  ```c
  int *x = &a[0];
  int *y = a;
  ```

## Multidimensional Arrays

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

![row-major order of a 3x3 matrix](figures/row-major.jpg){width=50%}

Although we visualize two-dimensional arrays as tables, that’s not the way they’re actually stored in computer memory. C stores arrays in _row-major order_, with row 0 first, then row 1, and so forth.

```c
int *row_ptr = rubiks_face[0];
int *square_ptr = &row_ptr[2]; // pointing to rubiks_face[0][2]
square_ptr++; // pointing to rubiks_face[1][0]
```

## Pointers: Arrays as Arguments/Parameters

When passed to a function, an array name is treated as a pointer. That is, what is passed to the function decays to a pointer to the first element.

```c
int find_largest(int a[], int n){
    int i, max = a[0];
    for (i = 1; i < n; i++)
        if (a[i] > max) max = a[i];
    return max;
}
int main() {
    ...
    find_largest(A, N); // A is not copied;
    ...                 // rather, a points to A[0]
    return 0;
}
```

## Pointers: Arrays as Arguments/Parameters

The size of an array is not inherently stored in the array itself; the only way to know/pass on how large the array is is to pass the length of the array alongside.

- Remember `argv`? It's an array of "strings" of length `argc`.
  ```c
  int main(int argc, char **argv) { return 0; }
  ```

### Strings

"Strings" are actually `char`-arrays, i.e. `char *`. They are _null-terminated_: their last values are the `\0` to indicate the end of the string; more about this when we discuss strings...

## Pointers: Arrays as Arguments/Parameters

1. An array used as an argument isn’t protected against change.
2. Just like with all structures, latency and bandwidth of passing an array are not affected by the size of the array.
3. An array parameter can be declared as a pointer if desired.

   - Although declaring a parameter to be an array is the same as declaring it to be a pointer, the same isn’t true for a variable.

     ```c
     int A[10]; // allocates memory for 10 integers
     int *a;    // allocates memory for a pointer, not an array
     ```

4. A function with an array parameter can be passed an array “slice”:

   ```c
   find_largest(&b[5], 10);
   ```

## Pointers: Arithmetic

Pointers are technically numbers, so you can add integers to them. Then, you can access other values with relative pointers.

- If `p` points to `A[i]`, other `A[j]` can be accessed by performing arithmetic on `p`.
- C supports exactly these three forms of pointer arithmetic.
  - pointer + integer; pointer - integer; or pointer - pointer
- Adding an integer `j` to a pointer `p` yields a pointer to the element `j` places after the one that `p` points to. That is, if `p` points to the array element `A[i]`, then `p+j` points to `A[i+j]`.

  - In other words, `A + i` is the same as `&A[i]` because both represent a pointer to element `i` of `A`.
  - Similarly, `∗(A+i)` is equivalent to `A[i]` because both represent `i`'th element of `A`.
  - Assuming 32-bit integers, each increment on a pointer will move 4 bytes down, giving us the pointer to the next element.

## Pointers: Arithmetic (Virtual Memory)

```c
#include <stdio.h>
int main () {
    int A[] = {1, 2, 4, 8, 16, 32, 64};
    for (int i=0; i<6; i++)
        printf("A[%d]: addr %x; val %d\n", i, &A[i], A[i]);
    return 0;
}
```

Note the 4-byte intervals of consecutive addresses in contiguous memory.

```c
A[0]: addr f4ec9730; val 1
A[1]: addr f4ec9734; val 2
A[2]: addr f4ec9738; val 4
A[3]: addr f4ec973c; val 8
A[4]: addr f4ec9740; val 16
A[5]: addr f4ec9744; val 32
```

## Pointers: Arithmetic (Virtual Memory)

```c
#include <stdio.h>
int main () {
    int A[] = {1, 2, 4, 8, 16, 32, 64};
    for (int i=0; i<6; i++)
        printf("A[%d]: addr %x; val %d\n", i, &A[i], A[i]);
    return 0;
}
```

Another run... the addresses (or rather, base addresses) always change, depends on memory.

```c
A[0]: addr b49d4720; val 1
A[1]: addr b49d4724; val 2
A[2]: addr b49d4728; val 4
A[3]: addr b49d472c; val 8
A[4]: addr b49d4730; val 16
A[5]: addr b49d4734; val 32
```

## Pointers & Memory

Arnold's Examples: [`mcs.utm.utoronto.ca/~209/23s/lectures/src/c/memory.c`](https://mcs.utm.utoronto.ca/~209/23s/lectures/src/c/memory.c)
