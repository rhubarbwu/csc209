# I/O Models

## I/O Models

Let's review some different I/O models...

- blocking I/O
- nonblocking I/O
- signal driven I/O (`SIGIO`)
- I/O multiplexing (`select` and `poll`)
- asynchronous I/O (the POSIX `aio_functions`)

Most of the time, there are two distinct phases of input operation.

1. Waiting for data to be ready (arriving from the network to the kernel's buffer).
2. Copying the data from the kernel to the process (from kernel's buffer to application's buffer).

### Source

\small

[`www.masterraghu.com/subjects/np/introduction/unix_network_programming_v1.3/ch06lev1sec2.html`](https://www.masterraghu.com/subjects/np/introduction/unix_network_programming_v1.3/ch06lev1sec2.html)

## I/O Models: Blocking

![a blocking I/O model; based on Haviland 7.1.6](lec11/io_blocking.svg)

## I/O Models: Non-Blocking

![a non-blocking I/O model; based on Haviland 7.1.6](lec11/io_nonblock.svg)

## I/O Models: Signal-Driven

![a signal-driven I/O model; based on Haviland 7.1.6](lec11/io_signal.svg)

## I/O Models: Multiplexing

![a multiplexiing I/O model; based on Haviland 7.1.6](lec11/io_multiplex.svg)

## I/O Models: Asynchronous

![an async I/O model; based on Haviland 7.1.6](lec11/io_async.svg){height=75%}

## I/O Models: Comparison

![how the models compare in control flow; based on Haviland 7.1.6](lec11/io_comparison.svg){height=80%}
