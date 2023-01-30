# Memory & Arrays (review)

## Memory ($\rho$)

- The operating system manages the real memory based on hardware.
- From our perspective we're working with virtual memory on top.
- Bytes are typically the smallest unit of memory.
  - Each unit has an address, which is an integer-like numeric that can be operated on with integers.
- The address of a variable/struct is the address of its first byte.
- Local-scope variables are typically allocated memory on the _stack_.
- Dynamic allocation to the _heap_ is explicitly handled (seen later).

## Arrays: Declaration & Allocation ($\rho$)

Arrays are sequence of uniformly-sized elements stored in a contiguous region of memory. They're declared to contain types (such as characters or numerics) and an array size between brackets `[size]`.

```c
float A[65];
A[0] = 6.0;
A[1] = 3.141592654;
printf("%f\n", A[1]); // bad approx of pi
printf("%f\n", A[2]); // undefined
```

A normal declaration assigns a region of memory to the array, but doesn't normally re-initialize the values.

### Static Arrays

However, `static` arrays do initialize values as `0`.

```c
static long B[4];
printf("%ld\n", B[3]); // 0
```

## Arrays: Initialization ($\rho$)

Alternatively, you can directly initialize arrays with values.

- Values in the array beyond the initializer are initialized as `0`.

  ```c
  int csc209[4] = {2, 0, 9}; // csc209[3] == 0
  ```

- Due to type inference, the size of such declarations is optional.

  ```c
  int csc369[] = {3, 6, 9}; // size inferred
  ```

## Arrays: Bounds in Memory ($\rho$)

- C doesn’t require that subscript bounds be checked; if a subscript goes out of range, the program’s behavior is undefined.
- No run-time check of array bounds: behaviour exceeding bounds is undefined. If lucky, it might (appear to) work with no side effects.

  - Sometimes it'll do something random, harmless or not.
  - Worst-case, it might crash the program or OS.

  ```c
  int csc469 = {2, 2, 0, 8};
  csc469[4] = 1; // will likely crash with stack smashing
  ```

- **Warning:** It is the programmer’s responsibility to keep track of the size of an array! Take care not to violate the bounds of the array.

### Arrays: Arnold's Examples

[`[www]/lectures/src/c/arraysVarLength.c`](https://mcs.utm.utoronto.ca/~209/23s/lectures/src/c/arraysVarLength.c)

where `[www]` = [`mcs.utm.utoronto.ca/~209/23s/`](https://mcs.utm.utoronto.ca/~209/23s)

## Pointers: Arithmetic ($\rho$)

Pointers are technically numbers, so you can add integers to them. Then, you can access other values with relative pointers.

- If `p` points to `A[i]`, other `A[j]` can be accessed by performing arithmetic on `p`.
- C supports exactly these three forms of pointer arithmetic:
  - pointer + integer; pointer - integer; or pointer - pointer
- Adding an integer `j` to a pointer `p` yields a pointer to the element `j` places after the one that `p` points to. That is, if `p` points to the array element `A[i]`, then `p+j` points to `A[i+j]`.

  - In other words, `A + i` is the same as `&A[i]` because both represent a pointer to element `i` of `A`.
  - Similarly, `∗(A+i)` is equivalent to `A[i]` because both represent `i`'th element of `A`.
  - Assuming 32-bit integers, each increment on a pointer will move 4 bytes down, giving us the pointer to the next element.

## Pointers: Arithmetic (Memory Addresses) ($\rho$)

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

## Pointers: Arithmetic (Memory Addresses) ($\rho$)

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

## Pointers: Arnold's Examples

[`[www]/lectures/src/c/crazyPointers.c`](https://mcs.utm.utoronto.ca/~209/23s/lectures/src/c/crazyPointers.c)
[`[www]/lectures/src/c/pointersAndFunctions.c`](https://mcs.utm.utoronto.ca/~209/23s/lectures/src/c/pointersAndFunctions.c)

where `[www]` = [`mcs.utm.utoronto.ca/~209/23s/`](https://mcs.utm.utoronto.ca/~209/23s)
