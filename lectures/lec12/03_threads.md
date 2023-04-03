# Threads & `pthread.h`

## Processes: The Anatomy

![processes might share open files and/or pipes](lec12/process.svg){height=80%}

## Processes: The Downsides

Were processes not good enough?

- `fork()` is a heavyweight system call.
- Each process gets its own memory address space.
- Context-switching and synchronizing between processes are expensive.
- Communication between processes requires external kernel constructs (pipes, sockets, signals, etc.) which are also expensive.

So, what if we had multiple _threads_ of execution, all within a single process?

![how drake feels about processes and threads](lec12/drake.jpg){height=45%}

## Threads: An Introduction

Threads belong to a process and share the same PID and parent PID.

- Technically each process by default is running on a single thread.
- Threads of a process also share the heap and global variables.
- Each thread also has unique attributes:
  - Its own thread ID;
  - Its own `errno` variable;
  - Its own stack for local variables and function calls.

### Advantages of threads

- Communication between threads is cheap; they can share variables!
- Threads are “lightweight”; creation and switching are faster.
- Synchronization avoids the kernel.

### Specs of a CPU

The number of threads "on a CPU" refers to the number of high-level tasks that your CPU can do at a time. Use `htop` to monitor the compute load on each.

## Threads: The Anatomy

![threads share an address space and open files](lec12/threads.svg){height=80%}

## Multiprocessing vs. Multithreading

Both processes and threads perform multiple tasks concurrently. Here's how...

| Attribute            | Processes            | Threads    |
| -------------------- | -------------------- | ---------- |
| Program Counter      | Individual           | Individual |
| Stack (pointer)      | Individual           | Individual |
| Memory               | Individual           | Shared     |
| Address Space (code) | Individual           | Shared     |
| Open Files (FDs)     | Shared               | Shared     |
| Pipes & Sockets      | Individual or Shared | Shared     |

The stack and heap addresses are shared by threads of the same process, but each thread gets its own "mini-stack" within the stack space of that process.

### Which should you use?

Depends what tasks you're performing and their complexity and dependencies.

- Processes can do some things that threads (which are simpler) can't.
- Sometimes threads can perform the same operations more efficiently.

## Threads: Package `pthread.h`

POSIX threads (`pthread`'s) is the most commonly used thread package on UNIX systems (unavailable on Microsoft Windows). Just compile with the `-pthread` flag.

\small

```c
#include "pthread.h"
#include "sys/types.h" // provides the *_t types
/* this is just a subset of functions */
int       pthread_cancel(pthread_t tid);
int       pthread_create(pthread_t *restrict tid,
                          const pthread_attr_t *restrict attr,
                          void *(*start_routine)(void *),
                          void *restrict arg);
int       pthread_detach(pthread_t tid);
void      pthread_exit(void *retval);
int       pthread_join(pthread_t tid, void **retval);
pthread_t pthread_self(void);
```

### Alternatives in C++

`pthread` is a sensible standard and is rather feature-rich. But others exist:

- `std::thread`: pretty standard but requires C++11. Only basic features are portable.
- `boost::thread`: cross-platform but has external dependency.

## Threads: Creation

Within a process, you can create a new thread with `pthread_create()`.

```c
#include "pthread.h"
int pthread_create(pthread_t *restrict tid,
                          const pthread_attr_t *restrict attr,
                          void *(*start)(void *),
                          void *restrict arg);
```

- `tid` uniquely identifies a thread within a process and is returned by the function.
- `attr` specifies attributes, as priority, initial stack size
  - use `NULL` for defaults.
- `start` is a pointer to a function that starts the thread.
  - `arg` (usually heap/global) is the argument to `start`.

### Error handling

`pthread_create()` doesn't set `errno` but returns compatible error codes. You can use `stderr() to print error messages.

## Threads: Termination

A thread typically terminates when its `start()` function returns or any thread in the process calls `exit()`?

If that's the case, calling `exit()` from any thread will terminate all threads in the process. What if we just want the thread to terminate? You'd call `pthread_exit()`.

```c
#include "pthread.h"
void pthread_exit(void *retval);
int  pthread_cancel(pthread_t tid);
```

Another way for a thread to terminate is being cancelled by another thread using `pthread_cancel()`.

### A refresher on `(void *)`

What does `(void *)` mean? It's a flexible pointer to any arbitrary data structure.

- To use it as any data type, you'd cast it to some compatible type. Otherwise, you might undetermined/unexpected behaviour, or a segmentation fault.

## Threads: Joining

By default, a thread is _joinable_: it can be enjoined by another thread. Similarly to using `waitpid()` for child processes, `pthread_join()` waits for a thread to terminate, or returns immediately if the thread has already terminated.

```c
#include "pthread.h"
int pthread_join(pthread_t tid, void **retval);
```

- `tid`: ID of the thread to wait for.
- `retval`: if not `NULL`, stores the `(void *)` pointer returned by thread `tid` upon termination.

### Comparison to `waitpid()`

Both `wait()`/`waitpid()` and `pthread_join()` are blocking. But...

- There is no thread hierarchy. Any thread can use `pthread_join()` to wait for any other thread in the same process.
- There is no way to "join with any thread", i.e. no analogy to `wait()`.
- There is no equivalent to `WNOHANG`.

## Threads: Detaching

You can also detach a _joinable_ thread `tid` such that it'll be a non-joinable daemon thread. A "detached" thread will run as normal, but it'll release its resources to the system automatically without any thread calling `pthread_join()`.

- Trying to detach a _non-joinable_ thread results in `EINVAL`.

```c
#include "pthread.h"
int pthread_detach(pthread_t tid);
pthread_t pthread_self(void)
```

You can use `pthread_self(void)` to find the "parent" (there is no explicit hierarchy) thread, i.e. the one that created the current one.

```c
pthread_detach(pthread_self(void)); // frequent use-case
```

## Threads: Inner Function Arguments

We can pass any variable (including a structure or array) to our thread function.

```c
pthread_t thread_ID; int fd, result;
fd = open("afile", O_RDONLY);
result = pthread_create(&thread_ID, NULL, myThreadFcn, (void *)&fd);
if (result != 0)
    printf("Error: %s\n", strerror(result));
```

- It assumes the thread function knows what type it is and will cast it therein.
- This example is _bad_ if the main thread alters `fd` later.

A better solution might include `malloc()` to create memory for the variable.

1. Initialize variable’s value.
2. Pass pointer to new memory via `pthread_create()`.
3. Thread function `myThreadFcn()` releases memory when done.

## Threads: Inner Function Arguments

A better solution might include `malloc()` to create memory for the variable.

\small

```c
// 0. define struct for the variable
typedef struct myArg { int fd; char name[25]; } MyArg;

// 1. initialize variable’s value
MyArg *p = (MyArg *)malloc(sizeof(MyArg));
p->fd = fd; // assumes fd is defined
strncpy(p->name, "CSC209", 7);

// 2. passes pointer to new memory
pthread_t thread_ID;
int result = pthread_create(&thread_ID, NULL, myThreadFunc, (void *)p);

void *myThreadFunc(void *p) {
    MyArg *theArg = (MyArg *) p;
    write(theArg->fd, theArg->name, 7);
    close(theArg->fd); free(theArg);
    return NULL;  // 3. releases memory when done
}
```

## Threads: Program Models

Several common models for threaded programs exist:

- Manager/worker: a single manager thread assigns work to other threads, the workers. The manager typically handles all input and parcels out work to the workers.
- Pipeline: a task is broken into a series of suboperations, each of which is handled in series, but concurrently, by a different thread. Is like an automobile assembly line.
- Peer: similar to the manager/worker model, but after the main thread creates other threads, it participates in the work.

What kinds of parallelism are exhibited in these models?

## Threads: Disadvantages & Safety

We must be careful when multithreading.

- A bug in one thread can damage other threads.
- It's difficult to use signals with threads.
- All threads in a process must run the same program.
- Only thread-safe functions can be used.

### Thread-Safety

Due to global/static variables and heap memory, not all functions are thread-safe.

\small

| Unsafe        | Safe            |
| ------------- | --------------- |
| `ctime()`     | `ctime_r()`     |
| `gmtime()`    | `gmtime_r()`    |
| `gethost*()`  |                 |
| `localtime()` | `localtime_r()` |
| `rand()`      | `rand_r()`      |
| `strtok()`    | `strtok_r()`    |

\normalsize

Notice a pattern in these function names? The `_r` stands for "re-entrant".
