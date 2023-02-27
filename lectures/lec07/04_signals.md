# Signals

## Signals: An Introduction

- Signals are unexpected, asynchronous events that can happen at any time.
- Unless you make special arrangements, most signals terminate your process.
- Signals are a basic form of **inter-process communication**.
- You have already been sending signals through the shell, e.g.,
  via the `ctrl+c` and `ctrl+z` key combinations.
  - What does `ctrl+c` do?
    - The terminal sends the `SIGINT` signal to the process.
    - By default, `SIGINT` ("Interrupt from keyboard") terminates the process.
  - What about `ctrl+z`?
    - Similarly, `ctrl+z` triggers a `SIGTSTP` signal ("Stop typed at terminal")

### The full list

On Linux, you can find a list in `/usr/include/bits/signum.h`.

## Signals: Punishment for Misbehaviour

The kernel sends several other signals to terminate processes when
a program misbehaves:

- `SIGSEGV` Invalid memory reference
- `SIGFPE` Floating-point exception
- `SIGILL` Illegal instruction

There are also the user-defined signals `SIGUSR1` and `SIGUSR2` that we can use for our own purposes. By default they terminate the process.

## Signals: Sending from the Shell

To send a signal `SIGNAME` to one or more processes given their process ID(s) `pid`'s, use `kill`. Why is called `kill`?

```sh
$ kill -SIGNAME <pids>
```

For example:

```sh
$ kill -SIGINT 11248
$ kill -SIGKILL 11248
```

Don't let the name `kill` fool you! It's generally for sending signals, but often used to send `SIGKILL`, hence the name.

### Unstoppable signals

`SIGKILL` and `SIGSTOP` cannot be caught, blocked, or ignored (see `man 7 signal`).

### Sending signals between processes

A process can send a signal to another process using the `kill()` system call.

## Signals: Handling

There are three options for handling signals:

1. Use the default action `SIG_DFL`.
2. Ignore the signal (i.e., the signal does nothing to your process).
3. Write a signal handler function, which will be called automatically upon receipt of a signal.

### Changing the Default Action

Two options for changing the default signal action:

2. The `signal()` C standard library function.
1. The `sigaction()` system call.

`signal()` is cross-platform but more limited in scope,
whereas `sigaction()` is more flexible but found only on
POSIX.1-compliant systems.

### More in the GNU C library manual

[`www.gnu.org/software/libc/manual/html_node/Signal-and-Sigaction.html`](https://www.gnu.org/software/libc/manual/html_node/Signal-and-Sigaction.html)

## Signals: Handlers

Related code is provided `signals.h`.

`signal` simply points each signal to a handler function.

```c
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
void handler() { printf("in signal handler\n"); }
int main() {
    for (int i = 0; i < 256; i++)
        signal(i, handler); // run handler() for every signal
    printf("starting\n");
    sleep(10);
    return 0;
}
```

### Arnold's Examples

[`mcs.utm.utoronto.ca/~209/23s/lectures/arnold/w07/signals/4handler.c`](https://mcs.utm.utoronto.ca/~209/23s/lectures/arnold/w07/signals/4handler.c)

## Signals: Actions

Related code is provided `signals.h`.

`sigaction` is the name of a system call function.

```c
int sigaction(int sig,
    const struct sigaction *act,
    struct sigaction *oldact);
```

And it's also the name of the struct type that it takes as the 2nd and 3rd arguments:

```c
struct sigaction {
    void     (*sa_handler)(int);
    void     (*sa_sigaction)(int, siginfo_t *, void *);
    sigset_t sa_mask;
    int      sa_flags;
    void     (*sa_restorer)(void);
};
```

- `sa_handler` can be set to `SIG_IGN` (ignore), `SIG_DFL` (default action),
  or the address of a handler function (see `man 2 sigaction`).

## Signals: Actions (Example)

```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
int i = 0;
void handler(int signo) {
    fprintf(stderr, "Sig %d; total %d.\n", signo, ++i);
}
int main(void) {
    struct sigaction newact;
    sigemptyset(&newact.sa_mask);
    newact.sa_flags = 0;
    newact.sa_handler = handler;
    if (sigaction(SIGINT, &newact, NULL) == -1) exit(1);
    if (sigaction(SIGTSTP, &newact, NULL) == -1) exit(1);
    for(;;); //Infinite loop
}
```

## Signals: Limitations

Signals are useful, but there are some drawbacks.

### Information

- Signals convey no information, aside from what type of signal
  (e.g., `SIGINT`, `SIGUSR1`) it is.
- Generally only used to indicate abnormal conditions: Not
  for data exchange.

### Queuing

- Multiple instances of the same signal do not queue.
- If signal `X` is sent while a previously-sent signal `X` is pending,
  then the second `X` is lost.
- Example, if your process receives a `SIGCHLD` (Child stopped
  or terminated), it may be that only one child process has terminated,
  or that _multiple_ child processes have terminated.

## Aside: Types and Portability

What is a `sigset_t` anyway? Good opportunity to demonstrate the primary need for `typedef`: allowing us to write _portable code_.

From `x86_64-linux-gnu/bits/types/sigset_t.h`:

```c
typedef __sigset_t sigset_t;
```

From `x86_64-linux-gnu/bits/types/__sigset_t.h`:

```c
#define _SIGSET_NWORDS (1024 / (8 * sizeof (unsigned long int)))
typedef struct {
    unsigned long int __val[_SIGSET_NWORDS];
} __sigset_t;
```

The type `__sigset_t` might be declared differently depending on your system, but code you write will be portable thanks to `typedef`. Try searching for the `#include` directive in `signal.h`.

## Signal: Sets as Bit Vectors

A signal set (`sigset_t`) is a _bit vector_ that specifies the set of signals to block; operate on them using the following standard library functions:

1. `int sigemptyset(sigset_t *set);`
2. `int sigfillset(sigset_t *set);`
3. `int sigaddset(sigset_t *set, int signo);`
4. `int sigdelset(sigset_t *set, int signo);`
5. `int sigismember(const sigset_t *set, int signo);`

See `man 3 sigsetops` for usage.

Note: recall the earlier discussion on bitwise operators.

## Signals: Masking During Program Execution

As our program may receive signals spontaneously at any time, we may need to block some signals from being delivered at an inopportune moment (e.g., writing to disk).

- This _temporary_ block is different from ignoring a signal entirely.
- Use the `sigprocmask()` system call to examine or change the set of blocked signals, via a _mask_ (i.e., a bit vector representing a set of signals).

Here's an example:

```c
$ sigset_t set, oldset;
$ sigemptyset(&set);
$ sigaddset(&set, SIGINT);
$ sigprocmask(SIG_BLOCK, &set, &oldset);
/*... Critical operation ...*/
$ sigprocmask(SIG_SETMASK, &oldset, NULL);
```

## Signals: Masking During Handler Execution

From `man 2 sigaction`:

> sa_mask specifies a mask of signals which should be blocked (i.e.,
> added to the signal mask of the thread in which the signal handler is
> invoked) during execution of the signal handler. In addition, the
> signal which triggered the handler will be blocked, unless the
> SA_NODEFER flag is used.

### Happy Birthday Example (Adapted from Karen's)

[`mcs.utm.utoronto.ca/~209/23s/lectures/rupert/w07/birthday.c`](https://mcs.utm.utoronto.ca/~209/23s/lectures/rupert/w07/birthday.c)

- Try uncommenting both `sigaction` calls.
  - You might notice you're stuck!
  - Open another terminal and use `kill`.
- Then, for `SIGINT`, try using `sigaddset` instead of `sigaction`.

## Signals: Async Safety

From `man 7 signal-safety`:

> Suppose a program is in the middle of a call to a `stdio`
> function such as `printf` where the buffer and associated variables have
> been partially updated. If, at that moment, the program is interrupted
> by a signal handler that also calls `printf`, the second call to
> `printf` will operate on inconsistent data, with unpredictable results.

### Happy Birthday Example (Adapted from Karen's)

[`mcs.utm.utoronto.ca/~209/23s/lectures/rupert/w07/birthday.c`](https://mcs.utm.utoronto.ca/~209/23s/lectures/rupert/w07/birthday.c)

- Uncomment the `printf` and `sleep` calls in the infinite loop.

## Signals: Writing to a Broken Pipe

Remember this?

```sh
$ ssh cslab
rupert@apps0:~$ client_loop: send disconnect: Broken pipe
```

- This happens when an `ssh` connection is left in-active for a long time.

`SIGPIPE` is sent to a process that tries to write to a pipe or to a _socket_ that does not have any readers.

More about processes and pipes later...

## Signals: Other Ways of Sending

- Recall `grep` searches for matching patterns.
  - `pgrep` search PID's for matching program names.
- For `kill` sends signals by PID(s).
  - `pkill` sends signals by matching program names.
    ```sh
    $ pkill -SIGINT hbd
    $ pkill hbd
    ```
- Some other applications like `htop` also allow sending signals.
