# Dynamic Memory

## Dynamic Memory: Pointers

1. `&` "returns" the address of any _named_ variable, `*` dereferences any _address_.
2. **Only** for variable declaration, `*` serves to **identify** variables that are pointers.
3. When reading/writing a pointer variable without dereferencing, you are reading/writing the **address** contained in the pointer.

### Casting Pointers

Arbitrary pointers can be cast as typed pointers. What does the following print?

```c
#include <stdio.h>
int main() {
    int x = 0x00616263; char *y = (char *)&x;
    printf("%s\n", y); // cba
    return 0;
}
```

- How? See [ASCII Table](http://www.asciitable.com/)
- Notice the ordering of the bytes.
- You are expected to understand hexadecimal...

## Dynamic Memory: Variables

### Local Variables

- Local variables are allocated in the function's _stack frame_.
  - In `gdb`, `backtrace` prints list of stack frames, tracing from currently-executing function up to `main()`.
- When a function returns, its stack frame is deallocated.
  - The freed-up space on the stack can be re-used by a future function that is called.

### Global Variables

- Global variables are stored in another region of memory.
  - Includes read-only _string literals_.
- These remain in memory for the entire duration that the program is running.

### Dynamically Allocated Variables

- Memory is allocated on the heap, referenced by a pointer.
- Persists on the heap even after the allocating function returns.

---

![memory model](figures/memory_model.svg){width=62%}

## Dynamic Memory: Allocation

The most basic structure to allocate memory for is an array(list).

### In Java

This is done automatically when creating objects.

```java
ArrayList createArray() {
  ArrayList a = new ArrayList();
  return a;
}
```

### In C

You'll have to be explicit using `malloc`, which takes the number of bytes for the structure and returns a pointer to it.

```c
int *createArray() {
  int *a = malloc(sizeof(int)*ARRAY_LEN);
  return a;
}
```

## Dynamic Memory: Allocation with `malloc`

The C library function `malloc` allocates the requested memory (of `size` bytes) and returns a `void` pointer to it.

```c
void *malloc(size_t size);
```

This function returns a `void *` to the allocated memory or `NULL` if the request fails.

- The pointer generally needs to be cast to be used as a typed pointer.
- A return value of `NULL` is often a result of running out of memory.

```c
char *str = (char *) malloc(15);
if (str == NULL) exit(1); // probably out of memory
strcpy(str, "tutorialspoint");
printf("String = %s,  Address = %u\n", str, str);
```

Use `top`/`*top` to see how much memory your system has. Then try to allocate more.

### Source

[`www.tutorialspoint.com/c_standard_library/c_function_malloc`](https://www.tutorialspoint.com/c_standard_library/c_function_malloc)

## Dynamic Memory: Allocation with `calloc`

If you allocated with `malloc`, the memory region might contain garbage. `calloc` is a way to allocate memory while zeroing it too.

```c
void *calloc(size_t nitems, size_t size);
```

It has different parameters, allocating space for `nitems` elements, each of `size` bytes.

```c
int n = 1000000;
long long *a = calloc(n, sizeof(long long)); // array of n long long
printf("from calloc\n");
for (int i = 0; i < n; i++)
    printf("%lld ", b[i]); // a bunch of 0s
printf("\n");
free(a);
```

`calloc` is not commonly used because most scenarios don't require `0`-initialization, and doing so introduces computational costs, especially for large pieces of memory.

## Dynamic Memory: Reallocation with `realloc`

The C library function `realloc` attempts to resize to `size` bytes the memory block pointed to by `ptr` that was previously allocated with a call to `malloc`, `calloc` or `realloc`

```c
void *realloc(void *ptr, size_t size)
```

- If `ptr` is `NULL`, a new block is allocated and a pointer to it is returned.
- If `size` is `0` and `ptr` points to an existing block of memory, the memory block pointed by `ptr` is deallocated and a `NULL` pointer is returned.

```c
str = (char *) realloc(str, 25);
strcat(str, ".com");
printf("String = %s,  Address = %u\n", str, str);
```

### Source

[`www.tutorialspoint.com/c_standard_library/c_function_realloc`](https://www.tutorialspoint.com/c_standard_library/c_function_realloc)

## Dynamic Memory: Leaks

![it's very relatable](figures/free_realestate.jpg){width=78%}

## Dynamic Memory: Freeing

If we're done with an object, can we reclaim the memory space?

- In Java, the _garbage collector_ asynchronously frees up memory when an object is no longer referenced by any variable.
- In Rust, each referenced piece of memory has a lifetime declared at runtime, so there's no garbage to speak of.
- In C/C++, you have to collect your own garbage.

  - Use `free()` to free up allocated space that is no longer being used.
  - Failure to do so results in _memory leaks_, which unnecessarily occupy space.
  - These can occur if you lose references to these piece of memory.
  - Use `*top` (like `htop`) to check memory consumption.
  - Use `valgrind` to detect memory leaks.

### Arnold's Examples

[`mcs.utm.utoronto.ca/~209/23s/lectures/src/c/malloc.zip`](https://mcs.utm.utoronto.ca/~209/23s/lectures/src/c/malloc.zip)

## Dynamic Memory: Pointers to (and Arrays of) Pointers

Let's model the following $\mathbb R^K \mapsto \mathbb R^3$ linear system $\mathbf W^T \mathbf x + \mathbf b$.
$$\mathbf x \in \mathbb R^3, \qquad \mathbf W \in \mathbb R^{K \times 3}, \qquad \mathbf b \in \mathbb R^3$$
$\mathbf x$ and $\mathbf b$ are easy since they're vectors.

```c
int K = 1000;
double *x = malloc(K * sizeof(double));
double *b = calloc(K, sizeof(double));
```

But $\mathbf W$ requires more care. Same goes for batching $m$ inputs as $\mathbf X \in \mathbb R^{K \times m}$. You must allocate memory top-down.

```c
double **W = malloc(3 * sizeof(double *));
for (int i=0; i<3; i++)
    W[i] = malloc(K * sizeof(double));
```

And afterwards, you should free bottom-up.

```c
for (int i=0; i<3; i++) free(W[i]);
```

## Dynamic Memory: Persistence on the Heap

Stack memory declared in a scope is only accessible therein (including function calls). Otherwise, what's not caught at compile-time can result in runtime memory errors.

```c
int *get_stack_ptr() { int *ptr; return ptr; }
int main() {
    while (1) { int x = 0; } x = 1; // compile error
    int *ptr = get_stack_ptr();
    int y = *ptr; // seg fault
}
```

Heap memory persists after function calls return, to be accessed with pointers.

```c
int *get_heap_ptr() { int *ptr = malloc(sizeof(int)); return ptr; }
int main() {
    int *ptr = get_heap_ptr();
    int y = *ptr;
    return 0;
}
```

## Dynamic Memory: Exercises (Stack)

How big are these? Where do they live? And until when?

```c
void fun1(char c) {  // how big is c? where? until when?
    float f;         // how big is f? where? until when?
}
void fun2(int *i_ptr) {}  // how big is i_ptr? where? until when?
int main() {
    int i = 0;        // how big is i? where? until when?
    int *i_ptr = &i;  // how big is i_ptr? where? until when?

    char s[10] = {'h', 'i'};  // how big is s? where? until when?
    char *s_ptr = s;          // how big is s_ptr? where? until when?

    int is[5] = {4, 5, 2, 5, 1};  // how big is is? where? until when?
    fun2(i);
    return 0;
}
```

## Dynamic Memory: Exercises (Stack)

How big are these? Where do they live? And until when?

```c
void fun1(char c) {  // 1 on fun1 stack until fun1 returns
    float f;         // 4 on fun1 stack until fun1 returns
}
void fun2(int *i_ptr) {}  // 8 on fun2 stack until fun2 returns
int main() {
    int i = 0;        // sizeof(int) on main stack until program ends
    int *i_ptr = &i;  // 8 on main stack until program ends

    char s[10] = {'h', 'i'};  // 10 on main stack until program ends
    char *s_ptr = s;          // 8 on main stack until program ends

    int is[5] = {4, 5, 2, 5, 1};  // 20 on main stack until program ends
    fun2(i);
    return 0;
}
```

## Dynamic Memory: Exercises (Heap)

How about these? `malloc` and `free` make an appearance...

```c
void fun1(int **i_ptr_ptr) {  // what about i_ptr_ptr?
    *i_ptr_ptr = malloc(sizeof(int) * 7);  // what about *i_ptr_ptr?
}
int *fun2() {
    int *i_ptr;                   // what about i_ptr?
    i_ptr = malloc(sizeof(int));  // what about *i_ptr?
    return i_ptr;
}
int main() {
    int *i_ptr;  // what about i_ptr?
    fun(&i_ptr);
    free(i_ptr);
    i_ptr = fun2();
    free(i_ptr);
    return 0;
}
```

## Dynamic Memory: Exercises (Heap)

How about these? `malloc` and `free` make an appearance...

```c
void fun1(int **i_ptr_ptr) {               // 8 on fun1 stack until fun1 return
    *i_ptr_ptr = malloc(sizeof(int) * 7);  // 28 on heap until free call
}
int *fun2() {
    int *i_ptr;                   // 8 on fun2 stack until fun2 return
    i_ptr = malloc(sizeof(int));  // 4 on heap until free call
    return i_ptr;
}
int main() {
    int *i_ptr;  // 8 on main stack until program ends
    fun(&i_ptr);
    free(i_ptr);
    i_ptr = fun2();
    free(i_ptr);
    return 0;
}
```

## Dynamic Memory: Exercises (Addresses)

Try drawing the memory model of the following code.

```c
#include "stdio.h"
#include "stdlib.h"
void init(int *a1, int *a2, int n) {
    for (int i = 0; i < n; i++) { a1[i] = i; a2[i] = 2*i+1; }
}
int main() {
    int nums1[3], *nums2 = malloc(sizeof(int) * 3);
    init(nums1, nums2, 2);
    for (int i = 0; i < 3; i++) printf("%d %d\n", nums1[i], nums2[i]);
    free(nums2);
    return 0; }
```

- Heap: `0x23c` to `0x248`.
- Stack for `init`: `0x454` to `0x470`.
- Stack for `main`: `0x474` to `0x48c`.
- Let `??` represent garbage.

---

| Section            | Address | Value | Variable |
| ------------------ | ------- | ----- | -------- |
| Heap               | `0x23c` |       |          |
|                    | `0x240` |       |          |
|                    | `0x244` |       |          |
| ...                |         |       |          |
| Stack frame `init` | `0x454` |       |          |
|                    | `0x458` |       |          |
|                    | `0x45c` |       |          |
|                    | `0x460` |       |          |
|                    | `0x464` |       |          |
|                    | `0x468` |       |          |
|                    | `0x46c` |       |          |
|                    | `0x470` |       |          |
| Stack frame `main` | `0x474` |       |          |
|                    | `0x478` |       |          |
|                    | `0x47c` |       |          |
|                    | `0x480` |       |          |
|                    | `0x484` |       |          |
|                    | `0x488` |       |          |

---

| Section            | Address | Value                          | Variable   |
| ------------------ | ------- | ------------------------------ | ---------- |
| Heap               | `0x23c` | `1`                            |            |
|                    | `0x240` | `3`                            |            |
|                    | `0x244` | `??`                           |            |
| ...                |         |                                |            |
| Stack frame `init` | `0x454` | `0x474`                        | `a1`       |
|                    | `0x458` | `0x474`                        | `a1`       |
|                    | `0x45c` | `0x23c`                        | `a2`       |
|                    | `0x460` | `0x23c`                        | `a2`       |
|                    | `0x464` | `2`                            | `n`        |
|                    | `0x468` | ~~`0`~~, ~~`1`~~, `2`          | `i`        |
|                    | `0x46c` |                                |            |
|                    | `0x470` |                                |            |
| Stack frame `main` | `0x474` | `0`                            | `nums1[0]` |
|                    | `0x478` | `1`                            | `nums1[1]` |
|                    | `0x47c` | `2`                            | `nums1[2]` |
|                    | `0x480` | `0x23c`                        | `nums2`    |
|                    | `0x484` | `0x23c`                        | `nums2`    |
|                    | `0x488` | ~~`0`~~, ~~`1`~~, ~~`2`~~, `3` | `i`        |
