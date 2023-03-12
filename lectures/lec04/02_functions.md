# Functions (review)

## Functions: Arguments by Value ($\rho$)

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

### Full code

[`github.com/rhubarbwu/csc209/blob/master/lectures/lec04/arg_cast.c`](https://github.com/rhubarbwu/csc209/blob/master/lectures/lec04/arg_cast.c)

## Functions: Arguments by Value ($\rho$)

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

## Functions: We need pointers! ($\rho$)

- Easy access to and abstraction of complex structures.
- Allows reference to the same data when desired.
- Pointers consume less memory than deep copies.
- Convenient null values for initialization/error-checking.

### Pointer Arguments

Pointers allow you to pass primitives or structures by reference, rather than value. Instead of copying and passing the entire structure, copy/pass the pointer(s) in constant time.

## Functions: Arguments by Reference ($\rho$)

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

## Functions: Pointers to Pointers (to Pointers ...) ($\rho$)

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

## Functions: Pointers & Arrays ($\rho$)

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

## Functions: Arrays as Arguments/Parameters ($\rho$)

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

## Functions: Arrays as Arguments/Parameters ($\rho$)

The size of an array is not inherently stored in the array itself; the only way to know/pass on how large the array is is to pass the length of the array alongside.

- Remember `argv`? It's an array of "strings" of length `argc`.
  ```c
  int main(int argc, char **argv) { return 0; }
  ```

### Strings

"Strings" are actually `char`-arrays, i.e. `char *`. They are _null-terminated_: their last values are the `\0` to indicate the end of the string; more about this when we discuss strings...

## Functions: Arrays as Arguments/Parameters ($\rho$)

1. An array used as an argument isn’t protected against change.
2. Just like with all structures, latency and bandwidth of passing an array are not affected by the size of the array.
3. An array parameter can be declared as a pointer if desired.

   - Although declaring a parameter to be an array is the same as declaring it to be a pointer, the same isn’t true for a variable.

     ```c
     int A[10]; // allocates memory for 10 integers
     int *a;    // allocates memory for a pointer, not array
     ```

4. A function with an array parameter can be passed an array “slice”:

   ```c
   find_largest(&b[5], 10);
   ```

### Functions: Arnold's Examples

[`mcs.utm.utoronto.ca/~209/23s/lectures/src/c/functions/functions.c`](https://mcs.utm.utoronto.ca/~209/23s/lectures/src/c/functions/functions.c)
