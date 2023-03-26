# Practice with Multiplexing

## Practice with Multiplexing: Descriptor Sets

You can use these macro functions to manipulate/operate on `fd_set`'s:

\small

```c
#include "sys/select.h"
void FD_CLR(int fd, fd_set *set);    // remove fd from set
int  FD_ISSET(int fd, fd_set *set);  // check if fd in set
void FD_SET(int fd, fd_set *set);    // add fd to set
void FD_ZERO(fd_set *set);           // empty/zero out set
```

\normalsize

Suppose you have a server `S` and two clients `c1`, `c2`.

1. `S` should read on FDs `4` and `6` from `c1` and `c2`, respectively.
2. FD `3` on `S` should also be flagged for everything.
3. No other FDs should be marked as ready.
4. _Some time after_ calling `select()`, let's also mark FD `3` as not ready at all.

Try drawing a graph between `S`, `c1`, `c2` and writing the representative bit vectors for the sets right before and after `select()`. Then use the macro function calls to perform the above operations. Start here:

\small

```c
fd_set *readfds, *writefds, *exceptfds;
/* some code */
```

## Practice with Multiplexing: Descriptor Sets

Suppose you have a server `S` and two clients `c1`, `c2`.

\small

1. `S` should read on FDs `4` and `6` from `c1` and `c2`, respectively.
2. FD `3` on `S` should also be flagged for everything.
3. No other FDs should be marked as ready.
4. _Some time after_ calling `select()`, let's also mark FD `3` as not ready at all.

\normalsize

The representative bit vectors immediately before/after `select()`:

- `readfds`: `00011010...` $\rightarrow$ `00010000...`
- `writefds`/`exceptfds`: `00010000...` (don't change)

And the code would look like this:

\small

```c
fd_set *readfds, *writefds, *exceptfds; /* some code */
FD_ZERO(readfs); FD_ZERO(writefds); FD_ZERO(exceptfds); // why clear?
FD_SET(4, readfds); FD_SET(6, readfds);
FD_SET(3, readfds); FD_SET(3, writefds); FD_SET(3, exceptfds);
select(7, readfds, writefds, exceptfds, NULL); // why 7?
FD_ZERO(writefds);
```

\normalsize

What does a `timeout` of value `NULL` do?

## Practice with Multiplexing: Office Hours Analogy

Consider the following `man` page of an imaginary system call `office_hours()`.

\footnotesize

```c
OFFICE_HOURS(2)                 BSD System Calls Manual                 OFFICE_HOURS(2)

NAME: IS_CLR, IS_ISSET, IS_SET, IS_ZERO, office_hours

SYNOPSIS
    void IS_CLR(is, is_set *isset); int IS_ISSET(is, is_set *isset);
    void IS_SET(is, is_set *isset); void IS_ZERO(is_set *isset);
    int office_hours(is_set *instr, struct timeval window);

DESCRIPTION: office_hours() examines the schedules for the instructors in the instructor
set instrs to see which have office hours scheduled within the given window from the
current time. office_hours() replaces the instructor set with the subset of instructors
who have office hours in the given window.

RETURN VALUE: office_hours() returns the number of instructors from the is_set who have
office hours in the window, or -1 if an error occurs. If office_hours() returns with an
error, the descriptor sets will be unmodified and the global variable errno will be set
to indicate the error.
```

\normalsize

`is_set`, `office_hours()` and the (macro) functions are very similar analogs to `fd_set`, `select()`, and those in `sys/select.h`.

## Practice with Multiplexing: Office Hours Analogy

`is_set`, `office_hours()` and the (macro) functions are very similar analogs to `fd_set`, `select()`, and those in `sys/select.h`. Suppose that (like FDs) instructors are represented by small integers and there are instructors defined as follows:

\small

```c
#define ANDREW 1
#define ARNOLD 2
#define BAHAR 3
#define RENATO 4
#define RUPERT 5
#define TINGTING 6
```

\normalsize

You want help in CSC258 in the next 24 hours. Finish the program on the next slide, so that it calls `office_hours()` and then prints either the message "Andrew has office hours" or the message "Renato has office hours" or both messages as appropriate.

- As an additional exercise, properly check for errors on a system call by writing the code to give the
  conventional behaviour if `office_hours()` fails.

## Practice with Multiplexing: Office Hours Analogy (Solution)

\small

```c
#include "stdio.h"
#include "sys/select.h"
int main() {
    struct timeval window = {24 * 60 * 60, 0};

    // set up first argument to office_hours()
    is_set instrs; IS_ZERO(&instrs);
    IS_SET(ANDREW, &instrs);
    IS_SET(RENATO, &instrs);

    // call office_hours()
    if (office_hours(&instrs, window) == -1) {
        perror("office_hours");
        exit(1);
    }

    // print the appropriate message
    if (IS_ISSET(RENATO, &instrs)) printf("Renato has office hours\n");
    if (IS_ISSET(ANDREW, &instrs)) printf("Andrew has office hours\n");
    return 0;
}
```

## Practice with Multiplexing: Other Questions

Suppose your server is written to block on both `read()` and `write()` calls.

1. Recalling last lecture, how might a `write()` call block?
2. Why might you write it this way? How is it useful?
3. How do we solve this?

## Practice with Multiplexing: Other Questions

Suppose your server is written to block on both `read()` and `write()` calls.

1. Recalling last lecture, how might a `write()` call block?

   - _TCP buffers if a server is sending data faster than a client can handle._

2. Why might you write it this way? How is it useful?

   - _Example: your server is responsible for selling tickets and clients can buy as many as possible. These tickets could be unique and involve some very complex hash composed of a large number of bytes._
   - _Example: your server `S1` ingests local data and shards it across multiple remote clients. The datagrams sent include timestamps of shards' transmission times._

3. How do we solve this? _Use `select()` for both reads and writes._

4. Expanding on the latter example in Question 2, how might you write a second server downstream that concurrently reads from the clients and reconstructs the original data in order?
5. Same as Question 4, but reconstructing in reverse order.

## Practice with Multiplexing: Other Questions

Scenario: your server ingests local data and shards it across multiple remote clients. The datagrams sent include timestamps and precedence of shards.

How might you write a second server `S2` downstream that concurrently reads from the clients and reconstructs the original data in order? Here are some potential solutions:

1. Use blocking I/O and call `read()` on the clients sequentially.
   - But this is problematic if the clients have their own timeouts i.e. they shutdown after their FDs have been open/ready for a while.
2. Instead, use multiplexing I/O.

   1. Introduce an ordering index (timestamps were of transmission time, not precedence) into the datagrams.
   2. Call `select()`+`read()` to write into an ordered buffer.
   3. How do you know how big the buffer should be?
      - Use dynamic memory for and `memmove()` on the buffer. But this wouldn't work in the reverse order case...
      - Instead, have `S1` transmit the sizes of the shards to `S2`. The value represnting the lenth of a shard doesn't grow in memory footprint.
