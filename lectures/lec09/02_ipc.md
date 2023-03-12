## Pipes in C: Inter-Process Communication (IPC)

Recall that upon calling `fork()` the child process gets a copy of the parent process' file descriptor (FD) table. So if a pipe is created before the fork, the child process after the fact also has access to the pipe.

![`exec*()`](figures/exec.png){width=90%}

While `exec*()` doesn't create new processes, the same FD table is retained after the program is replaced (important because `fork()` and `exec*()` are often used together).

## Pipes in C: IPC from Child to Parent

Here's a simple program where a child writes to its parent.

\setstretch{1.0}

```c
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define MSG_SIZE 13
char *msg = "hello, world\n";
int main(void) {
char buf[MSG_SIZE]; int p[2];
    if (pipe(p) == -1) { perror("pipe"); exit(1); }
    if (fork() == 0) // child writes
        write(p[1], msg, MSG_SIZE);
    else { // parent reads from pipe, and prints to stdout
        read(p[0], buf, MSG_SIZE);
        printf("%s\n", buf);
    }
    return 0;
}
```

As an exercise, identify safety checks you might want to perform.

## Pipes in C: IPC between Children

You can communicate between children processes too!

\setstretch{1.0}

```c
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define MSG_SIZE 20
char *msg = "good morning, bruv\n";
int main(void) {
    char buf[MSG_SIZE]; int p[2];
    if (pipe(p) == -1) { perror("pipe"); exit(1); }
    if (fork() == 0) write(p[1], msg, MSG_SIZE);
    else if (fork() == 0) {
        sleep(1);
        read(p[0], buf, MSG_SIZE);
        printf("%s\n", buf);
    }
    return 0;
}
```

What could go wrong? What's missing from the code?

## Pipes in C: Closing

```c
#include <unistd.h>
int close(int fd);
```

Each process should `close()` usused file descriptors, especially with pipes.

- This can save a bit of memory.
- More importantly, `read()` can stall otherwise. If a process calls `read()` on a pipe, but there is no data ready to be read, it will **stall forever**, unless:
  - New data arrives on the pipe; or
  - **All** file descriptors (across all processes) referring to
    the write end of the pipe have been closed.

This means that not only should processes that never use an FD close them immediately, but all processes should close each existing FD after its last use.

Read more on `man 7 pipe`.

### Blocking system calls

`read()` is an example of a blocking system call. What happens to the process when `read()` stalls as described above?

## Pipes in C: Closing

Does this program ever terminate?

\setstretch{1.0}

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define MSG_SIZE 21
char *msg = "get me the generals!";
int main(void) {
    char buf[MSG_SIZE];
    int p[2];
    if (pipe(p) != 0) { perror("pipe"); exit(1); }
    if (fork() == 0) {
        read(p[0], buf, MSG_SIZE);
        exit(0);
    }
    int status; wait(&status);
    return 0;
}
```

Which pipes should be closed?

## Pipes in C: Closing

Does this program ever terminate? **No it wouldn't. Let's close some FDs...**

\setstretch{1.0}

```c
int main(void) {
    char buf[MSG_SIZE]; int p[2];
    if (pipe(p) != 0) { perror("pipe"); exit(1); }
    if (fork() == 0) {
        close(p[1]); // closing from the child
        read(p[0], buf, MSG_SIZE);
        exit(0);
    }
    close(p[1]); // closing from parent
    int status; wait(&status);
    return 0;
}
```

Which pipes should be closed? **The write end `p[1]` should be closed in both the parent and the child processes. After all (both) ends have been closed, read() will return the number of bytes thus far read.**

### Arnold's Example: `pipes2.c`

[`http://www.cs.toronto.edu/~arnold/209/12s/lectures/pipes/pipes2.c`](http://www.cs.toronto.edu/~arnold/209/12s/lectures/pipes/pipes2.c)

## Pipes in C: Some Trivia

Here are some facts from `man 7 pipe`.

### Under _I/O on pipes and FIFOs_:

1. Pipes have no message boundaries. If you `write()` to a pipe twice and then `read()` from it, don't assume that you will just receive the first chunk of data that you wrote.
2. `write()` is guaranteed to be _atomic_ only if the data being written is smaller than a certain number of bytes (on Linux, 4KB).

### Under _Pipe capacity_:

3. Pipes have a finite size: Default is 64KB on Linux.
4. If a `write()` would overflow a pipe, the process blocks (or fails, if `O_NONBLOCK` flag is used when creating the pipe) until another process empties some of the data from the pipe by calling `read()`.
