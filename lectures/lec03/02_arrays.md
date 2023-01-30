# Memory & Arrays

## Memory

- The operating system manages the real memory based on hardware.
- From our perspective we're working with virtual memory on top.
- Bytes are typically the smallest unit of memory.
  - Each unit has an address, which is an integer-like numeric that can be operated on with integers.
- The address of a variable/struct is the address of its first byte.
- Local-scope variables are typically allocated memory on the _stack_.
- Dynamic allocation to the _heap_ is explicitly handled (seen later).

## Arrays: Declaration & Allocation

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

## Arrays: Initialization

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
