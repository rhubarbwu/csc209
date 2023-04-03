# Safety & Synchronization

## Exploiting Parallelism with Multithreading

Threads can easily share information using global variables. Consider this example.

\small

```c
int S = 0, N = 1073741824;
void *increment_by_one(void *_) {
    for (int j = 0; j < N; j++) { long local = S; local++; S = local; }
}
int main() { increment_by_one(NULL); }
```

If we're impatient, we can dispatch `T` threads to compute the sum in parallel.

\small

```c
int S = 0, N = 1073741824;
int main(int argc, char **argv) {
    int T = atoi(argv[1]); N /= T;
    pthread_t TIDs[T];
    for (int t = 0; t < T; t++)
        pthread_create(&TIDs[t], NULL, increment_by_one, NULL);
    for (int t = 0; t < T; t++) pthread_join(TIDs[t], NULL);
    return 0;
}
```

We are calling `pthread_join()`, so this is reliable and consistent, right?

## Safety: Exploiting Parallelism is Risky!

No! Absolutely not. Consider many threads are probably running `increment_by_one()` at once, accessing `S` concurrently. In other words, `S` can have multiple "writers" and "readers" at the same time.

\small

```c
void *increment_by_one(void *_) {
    for (int j = 0; j < N; j++) {
        long local = S;  // read from S
        local++;
        S = local;       // write to S
    }
}
```

Here's a possible (problematic execution path) if there are `T`=2 threads:

1. Thread 1 increments `S` $2000$ times.
2. On iteration `j=2001`, it obtains the value of `S`, but then...
3. Thread 2 increments `S` $536870912$ times, and terminates.
4. Now thread 1 takes over, but writes $2001$ into `S`!

`S` won't always be the expected result $1073741824$; you can imagine it gets worse with more threads! If the behaviour is inconsistent/unpredictable, we call this a _race condition_.

## Safety: Critical Sections

We refer to the reading and writing of a shared resource as a _critical section_. In a perfect world, threads would not interfere with each other in these. One can characterize these phases of code as those with variables that can have at most one writer, or only readers.

Can we solve this by reducing the number of times a critical section is run?

\small

```c
void *increment_by_part(void *_) {
    long partial = 0;
    for (int j = 0; j < N; j++)
        partial += 1;
    S += partial;
}
```

\normalsize

It certainly reduces the number of times the critical section runs, but increases the odds that each one is compromised owing to the longer run-time of the for loop. Also, the consequences of a single interference can be worse!

## Synchronization: Locks & Mutexes

\footnotesize

```c
#include "pthread.h"
int pthread_mutex_init(pthread_mutex_t *mp, const pthread_mutexattr_t *attr);
int pthread_mutex_lock(pthread_mutex_t *mp);
int pthread_mutex_trylock(pthread_mutex_t *mp);
int pthread_mutex_unlock(pthread_mutex_t *mp);
int pthread_mutex_destroy(pthread_mutex_t *mp);
```

\normalsize

A _mutual exclusion (mutex)_ can ensure that only thread accesses a variable at a time.

- A mutex is always in one of two states: locked or unlocked.
- When unlocked, any thread can lock (often phrase "acquire lock on") the mutex.
- Any thread that tries to acquire a locked mutex is blocked until it's unlocked.
- Only the thread that locked the mutex is allowed to unlock it.

Since threads share memory, their process's mutex `mtx` is common between them.

\small

```c
pthread_mutex_t mtx; // can be global or local
int status = pthread_mutex_init(&mtx, NULL);
...
pthread_mutex_lock(&mtx);   // if acquired by thread x, thread y is blocked
...                         // access shared resource
pthread_mutex_unlock(&mtx); // x releases mtx, y can acquire as above
```

## Synchronization: Locks & Mutexes

The terms "mutex" and "lock" are often used interchangeably. Whether a mutex/lock `mtx` applies across processes (mutex) or just between threads of the same process (lock) depends if `mtx` is global or local.

\small

```c
// mtx can be a global mutex            // or a local lock
void *increment_by_one(void *_) {       void *increment_by_part(void *_) {
    for (int j = 0; j < N; j++) {           long local = 0;
        pthread_mutex_lock(&mtx);           for (int j = 0; j < N; j++)
        long local = S;                      local += 1;
        local++;                            pthread_mutex_lock(&mtx);
        S = local;                       S += local;
        pthread_mutex_unlock(&mtx);         pthread_mutex_unlock(&mtx);
    }                                   }
} // which function does it better?     // look at critical sections
```

\normalsize

Here, we can use either because we don't `fork()` any new processes (so all threads belong to the same process) or have other functions taking up stack space.

### Full code

[`github.com/rhubarbwu/csc209/blob/master/lectures/lec12/counting.c`](https://github.com/rhubarbwu/csc209/blob/master/lectures/lec12/counting.c)

## Synchronization: Locks & Mutexes (Example with Macros)

To write versions of your code with/out mutexes, you might wrap (macro-)conditional directives `#ifdef`/`#else`/`#endif` around your mutex-related code.

\small

```c
void *increment_by_part(void *_) {
    long local = 0;
    for (int j = 0; j < N; j++) local += 1;
#ifdef MUTEX
    pthread_mutex_lock(&mtx);
#endif
    S += local;
#ifdef MUTEX
    pthread_mutex_unlock(&mtx);
#endif
}
```

Then compile with the `MUTEX` macro with the `-D` compiler flag to use mutexes.

\footnotesize

```sh
$ wget raw.githubusercontent.com/rhubarbwu/csc209/master/lectures/lec12/counting.c
$ gcc -pthread -o counting counting.c           # without mutexes
$ gcc -DMUTEX -pthread -o counting counting.c   # with mutexes
```

\normalsize

See if there are any changes in accuracy or efficiency...

## Synchronization: But Wait! There's more!

![finding out there's more than mutexes](lec12/vince-cv.jpg){height=82%}

## Synchronization: Conditional Variables (CVs)

A _condition variable_ (CV) allows threads to notify others of changes in resources. A CV also allows threads to block waiting for such notification. Without a CV, threaded programs can be inefficient (e.g. looping to poll a variable).

CVs are essentially mutexes with wait-queues and have three core operations.

- `signal`: wakes up at least one thread waiting for the CV.
- `broadcast`: wakes up all threads waiting for the CV.
- `wait`: waits (blocks) until signaled by a CV.

### CVs and mutexes

A CV has an associated mutex, which must be locked by a thread before it calls `pthread_cond_wait()`.

- `pthread_cond_wait()` unlocks the mutex, blocks the thread, and (when the thread is later signaled) relocks the mutex.
- Unlocking the mutex and blocking the thread are _atomic_: no other thread can `signal` the CV between these two operations.

## Synchronization: And more...

![finding out there's more than mutexes and CVs](lec12/vince-sem.jpg){height=82%}

## Synchronization: Semaphores

![[`open4tech.com/rtos-mutex-and-semaphore-basics/`](https://open4tech.com/rtos-mutex-and-semaphore-basics/)](lec12/sem.jpg){height=50%}

Semaphores are like CVs with a counter. In other words, instead of a binary locking status, a semaphore tracks some counter variable, essentially a multiplexing lock.

- Often tracks how many threads/processes are currently in a critical section.
- But it could be used to represent anything else the programmer wants.

## Synchronization: Okay Stop!

![](lec12/sweat.jpg){width=100%}

## Synchronization: Not a Silver Bullet do (Synchronized) Threads Make

\small

Ideally, we synchronize our process/threads across their shared resources properly. Then, multithreading is useful but you can have too much of a good thing.

![output of `time` from `bash`, with respect to (logarithmic) number of threads; notice that real/sys times eventually start to increase when there are too many threads.](lec12/plots.png)

\small

**Takeaway:** Don't spin too many threads because they consume memory to maintain and can introduce synchronization latency, especially around mutexes/CVs/semaphores.

### Code, scripts, and logs

\small

[`github.com/rhubarbwu/csc209/tree/master/lectures/lec12`](https://github.com/rhubarbwu/csc209/tree/master/lectures/lec12)

## Parallel Computing: Beyond CSC209

\small

Much parallel programming -- CUDA, the cloud, ASICs, etc. -- awaits! And development of new architectures, platforms, and algorithms is ongoing in both application and research!

\singlespacing

\vspace{-1.5em}

### [CSC367: Parallel Programming](https://utm.calendar.utoronto.ca/course/csc367h5)

\footnotesize

Topics include computer instruction execution, **instruction-level parallelism**, memory system performance, task and data **parallelism**, **parallel models** (**shared memory**, message passing), **synchronization**, scalability and Amdahl's law, Flynn taxonomy, vector processing and **parallel computing architectures**.

\vspace{-.3em}

### [CSC369: Operating Systems](https://utm.calendar.utoronto.ca/course/csc369h5)

\footnotesize

The operating system as a control program and as a resource allocator. **Processes and threads, concurrency (synchronization/mutual exclusion/deadlock)**, processor, scheduling, memory management, file systems, and protection.

\vspace{-.3em}

### [CSC409: Scalable Computing](https://utm.calendar.utoronto.ca/course/csc409h5)

\footnotesize

We investigate computation in the large [...] to solve complex problems involving big data, serving large collections of users, in high availability, global settings. [...] Topics include caching, load balancing, **parallel computing and models**, redundancy, failover strategies, use of **GPUs**, and noSQL databases.

\vspace{-.3em}

### [ECE419: Distributed Computing](https://engineering.calendar.utoronto.ca/course/ece419h1)

\footnotesize

Heterogeneity, security, transparency, **concurrency**, fault-tolerance; networking principles; request-reply protocol; remote procedure calls; distributed objects; middleware architectures; CORBA; security and authentication protocols; distributed file systems; name services; **global states in distributed systems**; **coordination and agreement**; **transactions and concurrency control**; distributed transactions; replication.
