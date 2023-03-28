# Loading into a Process (`exec`)

## `exec`: What is it?

The `exec` functions (provided by `unistd.h`) load a new program into the current process image. The process
retains its original PID.

![`exec*()`](lec08/exec.png)

## `exec`: The Basic `execve`

The `execve()` system call loads a new program into a process’s memory. During this
operation, the old program is discarded, and the process’s stack, data, and heap are
replaced by those of the new program.

```c
#include "unistd.h"
extern char **environ; // what's this for?

int execve(const char *filename, char *const argv[], char *const envp[]);
```

- `v`: arguments are passed by array `argv` at run-time.
- `e`: extra arguments can be specified in `envp`.
- The most frequent use of `execve()` is in the child produced by a `fork()`.
- It never returns on success, but returns `-1` on error. Why?
- If it does return, you may want to use `perror` right after.

See: [`linux.die.net/man/2/execve`](https://linux.die.net/man/2/execve).

### Arnold's Examples

[`www.cs.toronto.edu/~arnold/209/12s/lectures/process/index.html`](http://www.cs.toronto.edu/~arnold/209/12s/lectures/process/index.html)

## `exec`: Front-Ends

Various C library functions, all with names beginning with exec, are layered on top of
the `execve()` system call. Each of these functions provides a different interface to the
same functionality. The front-end functions differ mainly in how they are called.

```c
#include "unistd.h"
extern char **environ;
int execve(const char *filename, char *const argv[], char *const envp[]);
// front-ends
int execl(const char *path, const char *arg, ...);
int execlp(const char *file, const char *arg, ...);
int execle(const char *path, const char *arg, ..., char * const envp[]);
int execv(const char *path, char *const argv[]);
int execvp(const char *file, char *const argv[]);
int execvpe(const char *file, char *const argv[], char *const envp[]);
```

See: [`linux.die.net/man/3/exec`](https://linux.die.net/man/3/exec).

### How do they stack?

Try tracing all of these functions in [`codebrowser.dev`](https://codebrowser.dev/).

## `exec`: Front-Ends

| Function    | Spec of Program File `(-, p)` | Spec of Arguments `(v,l)` | Source of Environment `(e,-)` |
| ----------- | ----------------------------- | ------------------------- | ----------------------------- |
| `execl()`   | pathname                      | list                      | caller's `environ`            |
| `execle()`  | pathname                      | list                      | `envp` argument               |
| `execlp()`  | filename + `$PATH`            | list                      | caller's `environ`            |
| `execv()`   | pathname                      | vector (array)            | caller's `environ`            |
| `execve()`  | pathname                      | vector (array)            | `envp` argument               |
| `execvpe()` | filename + `$PATH`            | vector (array)            | `envp` argument               |
| `execvp()`  | filename + `$PATH`            | vector (array)            | caller's `environ`            |

The characters in the function name `exec*` can be informative.

- `l`: arguments are statically known at at compile time.
- `v`: arguments are passed by array `argv` at run-time.
- `e`: extra arguments can be specified in `envp`.
- `p`: if the program isn't found in the current directory, it'll search in `$PATH`.


## `exec`: Safety & Security

Because this is C, there are some safety/security implications programmers should consider. Using `execlp` and `execvp` can be very dangerous
when used improperly.

![`exec*()`](lec08/fry.jpg){width=41%}
![`exec*()`](lec08/doubt.jpg){width=55%}

Why? What makes these two special?

- Hint: Section 27.2.1 of [The Linux Programming Interface](https://search.library.utoronto.ca/details?10030432) by Michael Kerrisk.

Try looking up other vulnerabilities of `exec` functions...

## `exec`: Practice!

1. What's the difference between these?

   ```c
   execle("/bin/ls", "ls", NULL, NULL);

   char* args[] = {"ls", NULL};
   execve("/bin/ls", args, NULL);
   ```

2. Write a program similar to `yes` that just prints `no` without the use of loops.
3. Write two programs `tick` and `tock` that `print()` their respective names and call each other after a second has passed (for a specified number of seconds).

   - You can't use loops.
   - You can use `sleep()`, `atoi()`, and `sprintf`.

4. If you're feeling brave, try to write a single binary to mimic `tick` and `tock`.

   - Hint: it's easier if the binary is store somewhere on the `$PATH`.
   - Hint: try using `argv[0]` just like for `no`.

### Sample Solution

[`github.com/rhubarbwu/csc209/blob/master/lectures/lec08/txck.c`](https://github.com/rhubarbwu/csc209/blob/master/lectures/lec08/txck.c)
