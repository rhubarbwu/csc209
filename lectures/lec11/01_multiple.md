# I/O Models for Multiple Sources

## Reading From Multiple Sources

Assume that a process `p0` has any two file descriptors open
for reading (e.g., from a socket, regular file, pipe). Keep in mind that `read()` is blocking.

![a process `p0` reading from two file descriptors `fd1`, `fd2`](lec11/two_fds.svg){height=50%}

If `p0` reads from `fdX`, it will block until `fdX` has
data ready to read. But what if the other `fdY` already has
data available to be read?

## Reading From Multiple Sources: Another Perspective

Another way to view the problem is the following.

```
while true
    accept a new connection
    for each existing connection
        read
        write
```

Which of the system calls might block indefinitely?

- `read()` and `accept()`

So what happens if there is only one connection?

- The program will stall until the one `read()` has available data.

And what if there are multiple connections?

- The program will stall on the first `read()` that doesn't have available data and not be able to operate on existing connections or accept new ones.

## Reading From Multiple Sources: Using `fork()`

`p0` can `fork` one child process per file descriptor to be read from;
each child calls `read` on one file descriptor and communicates data
to parent over a pipe.

![using a new server-side `pipe()`+`fork()` to handle each `read()`](lec11/two_fds_two_processes.svg)

Which should be called first: `read()` or `wait()`?

## Reading From Multiple Sources: Using `fork()` and Sockets

- It is common for server software to `fork()` a new process for
  each client that connects: SSH does exactly that.
- _Performance_ benefit: Solves the issue of blocking `read()` calls that
  we just discussed.
- _Security_ benefit: Each process has its own memory space, making it
  less likely for there to be a bug that allows one user to read
  confidential information that belongs to another user
- Drawback: Each process takes up memory.
