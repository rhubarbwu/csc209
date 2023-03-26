# Multiplexing I/O with `select()`

## Multiplexing I/O with `select()`

`select()` monitors several file descriptors (FDs) simultaneously, without needing to `fork()` children processors to handle them.

\small

```c
#include "sys/select.h"
int select(int nfds, fd_set *restrict readfds, fd_set *restrict writefds,
    fd_set *restrict exceptfds, struct timeval *restrict timeout);
```

\normalsize

- Arguments include the upper bound of FDs (`nfds`), sets of FDs to monitor (`readfds`, `writefds`, `exceptfds`) and a `timeout`.

This function **blocks** until some monitored FD is "ready" or when `timeout` exceeded. Then, it returns the number of FDs that are ready, or `-1` on error.

### Motivation

The bottom line is that we **never** want to block on any calls to `read()` or `accept()`.

- Otherwise, we risk the possibility of waiting forever, even when there might be data ready to be read from other FDs.
- Instead, we write our client/server programs to block **only** on `select()`.

## Multiplexing I/O with `select()`: Parameters

```c
int select(int nfds,
    fd_set *restrict readfds,
    fd_set *restrict writefds,
    fd_set *restrict exceptfds,
    struct timeval *restrict timeout);
```

`select()` takes several arguments...

1. `nfds`: The (exclusive) upper bound on the FDs that're monitored.
   - Set it to just above the highest-numbered FD of interest
   - If you're interested in FDs `3`, `9`, `50`, let `nfds` be `51`.
2. `readfds`: set of FDs to monitor for _reading_.
3. `writefds`: set of FDs to monitor for _writing_.
4. `exceptfds`: set of FDs to monitor for _exceptions_.
5. `timeout`: how long we're willing to wait for a given FD to be ready.

### More About Exceptions

\small
[`www.gnu.org/software/libc/manual/html_node/Out_002dof_002dBand-Data.html`](https://www.gnu.org/software/libc/manual/html_node/Out_002dof_002dBand-Data.html)

## Multiplexing I/O with `select()`: Readiness

`select()` **blocks** until some monitored FD is "ready". What does that mean?

### Ready for Reading (`readfds`)

a FD is ready for _reading_ when `read()` can be called _once_ without blocking; this happens on socket errors, or under the following condictions:

- there is data in the receive buffer to be read;
- end-of-file state (EOF) is detected on the FD;
- the socket is a listening socket and there's a pending connection.

Most of the time, we are interested in monitoring FDs in `readfds`.

### Ready for Writing (`writefds`)

FD is ready for _writing_ if there's space in the write buffer or a pending socket error.

### Ready for Exceptions (`exceptfds`)

In the TCP protocol, some data is _urgent_, so the receiver should process it immediately outside the buffer stream. a FD can have an exception condition when it has this "out-of-band" data.

## Multiplexing I/O with `select()`: Readiness is Level- or Edge-Triggered

When is an FD ready? In the following scenarios:

1. _Level-triggered_: when an operation (e.g. `read`) won't block; or
2. _Edge-triggered_: when there is new action on the FD since the last check.

`select()` is level-triggered: if you don't read everything, `select()` will keep telling you that the FD is ready

## Multiplexing I/O with `select()`: Descriptor Sets

\small

```c
#include "sys/select.h"
#define __FDS_BITS(set) ((set)->__fds_bits)
typedef struct { // for POSIX/Berkeley/BSD sockets
    __fd_mask __fds_bits[__FD_SETSIZE / __NFDBITS];
} fd_set;
```

\normalsize

File descriptor sets (`fd_set`'s) are similar to signal sets but typically implemented as an array of integers where each bit corresponds to a FD.

- Implementation is hidden in the `fd_set` data type (e.g. Windows doesn't use integer arrays).
- `FD_SETSIZE` is the number of FDs in the
  data type.
- The argument `nfds` specifies the number of FDs (counting from `0`) to test.

You can use these macro functions to manipulate/operate on `fd_set`'s:

\small

```c
#include "sys/select.h"
void FD_CLR(int fd, fd_set *set);    // remove fd from set
int  FD_ISSET(int fd, fd_set *set);  // check if fd in set
void FD_SET(int fd, fd_set *set);    // add fd to set
void FD_ZERO(fd_set *set);           // empty/zero out set
```

## Multiplexing I/O with `select()`: Timeout

`select()` returns when ( and blocks until) some monitored FD is "ready" or when `timeout` exceeded. The `timeout` specifies how long we're willing to wait for a FD to become ready.

```c
struct timeval {
    long tv_sec;    // seconds
    long tv_usec;   // microseconds
};
```

Depending on a value of the `timeval` struct, `select()` might wait.

- If timeout is `0`, test and return immediately.
- If timeout is `NULL`, wait forever (or until we catch a signal).
- Otherwise wait up to specified timeout.

## Multiplexing I/O with `select()`: A Variant

There exists a variant of `select()` called `pselect()`.

\small

```c
#define _POSIX_C_SOURCE 200112L
#include <sys/select.h>
int pselect(int nmsgsfds, fd_set *__restrict__ readlist,
           fd_set *__restrict__ writelist, fd_set *__restrict__ exceptlist,
           const struct timespec *__restrict__ timeout,
           const sigset *__restrict__ sigmask);
```

\normalsize

The key differences are as follows:

- `select()` takes a `timeval` while `pselect()` takes `timespec`.
  - What's the difference? _Their second members (`tv_usec`, `tv_nsec`) are in microseconds and nanoseconds, respectively._
- `pselect()` adds a `sigmask` argument. If `sigmask` is a null pointer, this is equivalent to `select()`.
  - What does this do? _It's a bit vector indicating which signals to ignore._
- Unlikely `select()`, `pselect()` cannot modify `timeout` upon success.
  - Why would you want to? _You can modify `timeout` to indicate remaining time._

Source: [www.ibm.com/docs/en/zos/2.2.0](https://www.ibm.com/docs/en/zos/2.2.0?topic=lf-select-pselect-monitor-activity-files-sockets-message-queues)

## Multiplexing I/O with `select()`: Some Examples/Demos

Arnold provided some examples:

- [`[www]/lectures/src/select/selectExample0.c`](https://mcs.utm.utoronto.ca/~209/23s/lectures/src/select/selectExample0.c)
- [`[www]/lectures/src/select/selectExample1.c`](https://mcs.utm.utoronto.ca/~209/23s/lectures/src/select/selectExample1.c)
- [`[www]/lectures/src/select/selectExample2.c`](https://mcs.utm.utoronto.ca/~209/23s/lectures/src/select/selectExample2.c)
- [`[www]/lectures/src/select/muffinman.c`](https://mcs.utm.utoronto.ca/~209/23s/lectures/src/select/muffinman.c)

`[www]` = [`mcs.utm.utoronto.ca/~209/23s`](https://mcs.utm.utoronto.ca/~209/23s)

### Chat Room Demo

And of course, chat rooms were a motivating use case..

- [`mcs.utm.utoronto.ca/~209/23s/lectures/src/select/charserver.c`](https://mcs.utm.utoronto.ca/~209/23s/lectures/src/select/charserver.c)
- [`mcs.utm.utoronto.ca/~209/23s/lectures/src/select/charserver2.c`](https://mcs.utm.utoronto.ca/~209/23s/lectures/src/select/charserver2.c)
- [`github.com/kirintwn/socket-chat-room`](https://github.com/kirintwn/socket-chat-room)
