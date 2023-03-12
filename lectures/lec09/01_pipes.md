# Pipes in C

## Pipes in C

In C programming, pipes are a one-way and first-in first-out (FIFO) mechanism for inter-process communication (IPC). Data is passed between two processes by connecting the output of one to the input of process using a pipe.

- Pipes live in main memory as "virtual files".
- Just like real files, pipes can be used by not only the process that creates them, but also their children.

### Are they the same as UNIX pipes?

You might recall that in the UNIX shell, we also used "pipes". Those (using the character `|`) are simpler, and redirect the output of one command as the input of another. Pipes in C are more complex and powerful than that!

## Pipes in C: Anatomy

A pipe `p` has two ends, a read end `p[0]` and a write end `p[1]`.

![pipes from/to a process](figures/pipes.jpg){width=65%}

These two ends are typically used on different processes, such as a parent and its child, or related children processes.

## Pipes in C: Creation

To create a pipe in C, the `pipe()` system call is used.

```c
#include "unistd.h"
int pipe(int filedes[2]);
```

Its parameters are two integers, each describing the file descriptor (FD) of an end of the pipe; these FDs are how C programmers interact with pipes.

```c
int p[2];   // usually they're just declared on the stack,
pipe(p);    // and the pipe() system call initializes them.
```

When called, `pipe()` finds the first two available positions in the process's open file table and allocates them for the two ends of the pipe. Returns `0` on success.

### Error Checking

Creating pipes might fail so checking is a good idea, even if you don't want to crash.

```c
if (pipe(p) < 0) {
    perror("failed to open pipe");
    exit(1); // you could do other handling instead...
}
```

## Pipes in C: Reading & Writing

```c
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
```

Once a pipe has been created, it can be used to communicate between processes. To send data through a pipe,

- One process writes data to the write end `write_fd` (or `p[1]`) of the pipe using the `write()` system call.
  ```c
  write(write_fd, data, sizeof(data));
  ```
- The other process can then read the data from the read end `read_fd` (or `p[0]`) of the pipe using the `read()` system call.
  ```c
  read(read_fd, buffer, sizeof(buffer));
  ```

The size given in the the `read()`/`write()` calls must be passed in the function call.

- Can the passed size be smaller than the actual size? What about larger?

## Pipes in C: On a Single Process

It's possible to operate on both ends of the same pipe in a single process.

\setstretch{1.0}

```c
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define MSG_SIZE 13
char *msg = "hello, world\n";
int main(void) {
    char buf[MSG_SIZE];
    int p[2];
    if (pipe(p) == -1) { perror("pipe"); exit(1); }
    write(p[1], msg, MSG_SIZE); // No error checking: Bad
    read(p[0], buf, MSG_SIZE); // No error checking: Bad
    write(STDOUT_FILENO, buf, MSG_SIZE);
    return 0;
}
```

This can be used as a form of memory.

### Arnold's Example: `pipes1.c`

[`www.cs.toronto.edu/~arnold/209/12s/lectures/pipes/pipes1.c`](http://www.cs.toronto.edu/~arnold/209/12s/lectures/pipes/pipes1.c)

## Pipes in C: Safe Communications

There are many safety/security considerations when using pipes. The major sources of bugs we've touched on before are:

- Strings manipulation functions.
- Process creation and loading: `fork()`/`exec*()`.
- Low-level file I/O: `write()`, `read()`.

Even if your code runs without errors, there might be unexpected/undesired behaviour and security compromises.

### Indicating the wrong number of bytes

You _can_ read/write the wrong number of bytes to a pipe. But should you?

```c
char buf[MSG_SIZE];
write(STDOUT_FILENO, buf, MSG_SIZE+1); // same as printf()
```

While `clang` wouldn't catch this mistake, `gcc` would! How?

#### Demo: `rainy.c`

[`github.com/rhubarbwu/csc209/blob/master/lectures/lec09/rainy.c`](https://github.com/rhubarbwu/csc209/blob/master/lectures/lec09/rainy.c)
