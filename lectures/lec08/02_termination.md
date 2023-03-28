# Termination & Status

## Termination & Status (`_exit`)

A process may terminate in two general ways:

1. abnormal termination
2. normal termination, using the `_exit()` system call.

```c
#include "stdlib.h"
void _exit(int status);
```

A _process exit status_ is saved to be reported back to the parent process via `wait` or `waitpid`. If the program exited, this status includes as its low-order 8 bits the _program exit status_.

![](lec08/exitstatus.svg){width=90%}

[Source: The GNU C Library](https://www.gnu.org/software/libc/manual/html_node/Termination-Internals.html)

## Termination & Status (`exit`)

Programs generally don’t call `_exit()` directly. Instead the `exit()` library function is called. `exit()` performs some cleanup (e.g., flush `stdio` streams) and calls `_exit()`, which sets the process exit status (or termination status), and terminates the process.

```c
#include "stdlib.h"
void exit(int status);
```

The `status` argument given to `_exit()` and `exit()` is the program’s exit status, which becomes part of the process’s exit status.

### What's the Difference?

```c
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

int main() {                int main() {
    printf("Hi");               printf("Hi");
    _exit(0);                   exit(0);
}                           }
```

## Termination: Exit vs. Return

Inside `main()`, `return` and `exit()` are nearly equivalent (save some edge cases):

1. The return value of `main()` is the _program exit status_ passed to `exit()`.
2. `exit()` performs some cleanup (e.g., flush `stdio` streams) and calls `_exit()`.
3. `_exit()` sets the _process exit status_, or _termination status_, and terminates the process.

Outside of `main()`, use `exit()` to terminate the process.

### Exit Status Conventions

Return `0` on success, any other value on error.

[But if you're a real GNU/Linux geek...](https://www.gnu.org/software/libc/manual/html_node/Exit-Status.html)

> A general convention reserves status values 128 and up for
> special purposes. In particular, the value 128 is used to
> indicate failure to execute another program in a subprocess.

## Termination: Orphans & Zombies

The lifetimes of parent and child processes are usually not the same: either the parent outlives the child or vice versa.

### Orphan Processes

- A terminating process may be a parent; in that case all of its children processes become orphans.
- The kernel ensures all of the orphaned processes are adopted by `init`.

### Zombie Processes

When a child terminates before its parent, the parent should still be permitted to perform a `wait()` at some later time to determine how the child terminated.

- The kernel deals with this situation by turning the child into a _zombie_, a process that is waiting for its parent to accept its return code.
- When a process become a zombie, most of the resources held by it are released back to the system except an entry in the kernel’s process table recording the child’s PID, termination status, and resource usage statistics.
