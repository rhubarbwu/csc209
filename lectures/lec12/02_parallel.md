# Parallel Computing

## Parallelism: Multiprocessing

Why do we use multiple processes? There's a limit to what single-processing (and single-core technology) can do. We want:

- Isolation: each process and its data can be protected from the others.
- Parallelism: if our computer has multiple processors, then each processor can be running a process!

Ultimately, multiprocessing takes advantage of the fact that many things can be done at the same time.

## Parallelism: Types of Parallel Computing

There are several granularities to which we might apply parallelism.

1. Bit-level: operate on many bits in parallel.
2. Instruction-level: operate with many instructions in parallel.
3. Data-level: read/write several chunks of data in parallel.
4. Task-level: perform many higher-level tasks/programs in parallel.
5. Machine-level: federate or decentralize tasks across several machines/nodes such as servers and clients.

Which types are the result of multiprocessing via `fork()`?

- `fork()` typically resembles task-level parallelism.

What about `fork()`+`select()`?

- When `select()` is used for child process that read from files/pipes/sockets, there is some data-level parallelism.

Can you think of examples for the other types?

## Parallelism: Synchronicity

Parallel solutions might require some coordination or synchronization if there exist inter-process dependencies. Otherwise, asynchronous parallelism might be possible.

1. Does multiprocessing via `fork()` resemble in synchronous or asynchronous parallelism?

   - By default child process as result of `fork()` are asynchronously parallel.

2. What happens when we invoke a blocking call like `wait()` or `read()`?

   - Processes are synchronized at the blocking call.

3. Why asynchronous parallelism?

   - When processes don't depend on each other, asynchronous parallelism allows them to make more progress or "throughput".
