# Error Number (`errno`)

## Error Number (`errno`)

The macro `errno` is implemented (system-dependently) as variable `int`.

```c
#define errno /*implementation-defined*/
```

- At the start of a program, `errno` has the value `0`.
- Library functions can write strictly positive `int` to `errno` to indicate the last error that occurred.
  - `perror` is provided by `stdio.h` and used to print the associated description.
    ```c
    FILE *f = fopen("non_existent", "r");
    if (f == NULL)
        perror("fopen() failed");
    else
        fclose(f);
    ```
    Possible output:
    ```sh
    fopen() failed: No such file or directory
    ```
  - `strerror` likewise produces the description string.

## Error Number (`errno`)

The macro `errno` is implemented (system-dependently) as variable `int`.

```c
#define errno /*implementation-defined*/
```

- `errno` doesn't force your program to fail, but you can use it as an excuse to.
- In addition to `gdb`, `valgrind`, it can be used as validation (more useful than `assert.h`) to aid debugging.

### Issues pre-C11

Previously `errno` was supposedly a macro but also unspecified whether it was a macro or declared identifier. As of C11, `errno` is a macro.

### More reading

See descriptions and demo in [`en.cppreference.com/w/c/error/errno`](https://en.cppreference.com/w/c/error/errno).
